package cp2024.solution;

import cp2024.circuit.CircuitValue;
import java.util.concurrent.*;

public class ParallelCircuitValue implements CircuitValue {
    private final Future<Boolean> future;
    private final Exception exception;

    public ParallelCircuitValue(Future<Boolean> future) {
        this.future = future;
        this.exception = null;
    }

    public ParallelCircuitValue(Exception exception) {
        this.future = null;
        this.exception = exception;
    }

    @Override
    public boolean getValue() throws InterruptedException {
        if (exception != null) {
            throw new InterruptedException(exception.getMessage());
        }
        try {
            return future.get();
        } catch (InterruptedException | ExecutionException | CancellationException e) {
            throw new InterruptedException();
        }
    }

}