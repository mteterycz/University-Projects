package cp2024.solution;

import cp2024.circuit.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class ParallelCircuitSolver implements CircuitSolver {
    private final ExecutorService executor = Executors.newCachedThreadPool();
    private volatile boolean acceptComputations = true;

    @Override
    public CircuitValue solve(Circuit c) {
        if (!acceptComputations) {
            return new ParallelCircuitValue(new InterruptedException("Solver stopped"));
        }
        Future<Boolean> future = executor.submit(() -> computeNode(c.getRoot()));
        return new ParallelCircuitValue(future);
    }

    @Override
    public void stop() {
        acceptComputations = false;
        executor.shutdownNow();
    }

    private Boolean computeNode(CircuitNode node) throws Exception {
        if (Thread.currentThread().isInterrupted()) {
            throw new InterruptedException();
        }
        return switch (node.getType()) {
            case LEAF -> ((LeafNode) node).getValue();
            case NOT -> !computeNode(node.getArgs()[0]);
            case AND -> computeAnd(node.getArgs());
            case OR -> computeOr(node.getArgs());
            case GT -> computeGt((ThresholdNode) node);
            case LT -> computeLt((ThresholdNode) node);
            case IF -> computeIf(node.getArgs());
        };
    }

    private Boolean computeOr(CircuitNode[] args) throws Exception {
        CompletionService<Boolean> completionService = new ExecutorCompletionService<>(executor);
        List<Future<Boolean>> futures = new ArrayList<>();
        for (CircuitNode arg : args) {
            futures.add(completionService.submit(() -> computeNode(arg)));
        }
        try {
            for (int i = 0; i < args.length; i++) {
                Future<Boolean> future = completionService.take();
                if (future.get()) {
                    for (Future<Boolean> f : futures) {
                        f.cancel(true);
                    }
                    return true;
                }
            }
            return false;
        } catch (CancellationException e) {
            throw new InterruptedException("Task canceled");
        }
    }

    private Boolean computeAnd(CircuitNode[] args) throws Exception {
        CompletionService<Boolean> completionService = new ExecutorCompletionService<>(executor);
        List<Future<Boolean>> futures = new ArrayList<>();
        for (CircuitNode arg : args) {
            futures.add(completionService.submit(() -> computeNode(arg)));
        }
        try {
            for (int i = 0; i < args.length; i++) {
                Future<Boolean> future = completionService.take();
                if (!future.get()) {
                    for (Future<Boolean> f : futures) {
                        f.cancel(true);
                    }
                    return false;
                }
            }
            return true;
        } catch (CancellationException e) {
            throw new InterruptedException("Task canceled");
        }
    }

    private Boolean computeGt(ThresholdNode node) throws Exception {
        CompletionService<Boolean> completionService = new ExecutorCompletionService<>(executor);
        List<Future<Boolean>> futures = new ArrayList<>();
        CircuitNode[] args = node.getArgs();
        int threshold = node.getThreshold();
        for (CircuitNode arg : args) {
            futures.add(completionService.submit(() -> computeNode(arg)));
        }
        int trueCount = 0;
        try {
            for (int i = 0; i < args.length; i++) {
                Future<Boolean> future = completionService.take();
                if (future.get()) {
                    trueCount++;
                    if (trueCount > threshold) {
                        for (Future<Boolean> f : futures) {
                            f.cancel(true);
                        }
                        return true;
                    }
                }
            }
            return false;
        } catch (CancellationException e) {
            throw new InterruptedException("Task canceled");
        }
    }

    private Boolean computeLt(ThresholdNode node) throws Exception {
        CompletionService<Boolean> completionService = new ExecutorCompletionService<>(executor);
        List<Future<Boolean>> futures = new ArrayList<>();
        CircuitNode[] args = node.getArgs();
        int threshold = node.getThreshold();
        for (CircuitNode arg : args) {
            futures.add(completionService.submit(() -> computeNode(arg)));
        }
        int trueCount = 0;
        try {
            for (int i = 0; i < args.length; i++) {
                Future<Boolean> future = completionService.take();
                if (future.get()) {
                    trueCount++;
                }
                if (trueCount >= threshold) {
                    for (Future<Boolean> f : futures) {
                        f.cancel(true);
                    }
                    return false;
                }
            }
            return true;
        } catch (CancellationException e) {
            throw new InterruptedException("Task canceled");
        }
    }

    private Boolean computeIf(CircuitNode[] args) throws Exception {
        Future<Boolean> condition = executor.submit(() -> computeNode(args[0]));
        Future<Boolean> positive = executor.submit(() -> computeNode(args[1]));
        Future<Boolean> negative = executor.submit(() -> computeNode(args[2]));
        try {
            while (!condition.isDone()) {
                if (positive.isDone() && negative.isDone()) {
                    boolean pos = positive.get();
                    boolean neg = negative.get();
                    if (pos == neg) {
                        condition.cancel(true);
                        return pos;
                    } else {
                        return condition.get() ? pos : neg;
                    }
                }
            }
            if (condition.get()) {
                negative.cancel(true);
                return positive.get();
            } else {
                positive.cancel(true);
                return negative.get();
            }
        } catch (CancellationException e) {
            throw new InterruptedException("Task canceled");
        }
    }
}