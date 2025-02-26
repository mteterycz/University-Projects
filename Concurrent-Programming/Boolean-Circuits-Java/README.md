# Concurrent Boolean Expression Evaluation

## Description
The program implements concurrent evaluation of Boolean circuits representing logical expressions in a tree-like structure. Each expression consists of nodes representing logical operators (AND, OR, NOT, IF) and threshold operators (GT, LT), as well as constant values (true, false).

The computations are performed concurrently, meaning that subexpressions can be evaluated in parallel. Additionally, the system supports lazy evaluation, which avoids unnecessary computations when the result is already known.

## Structure
- **Circuit** – A class representing a Boolean circuit, containing the root of the expression tree.
- **CircuitNode** – An abstract class representing a node in the expression tree.
- **LeafNode** – A node representing constant Boolean values (true/false).
- **ThresholdNode** – A node representing threshold operators (GT, LT).
- **CircuitSolver** – An interface for computing circuit values.
- **CircuitValue** – An interface enabling retrieval of the computation result.

## CircuitSolver Interface
```java
public interface CircuitSolver {
    public CircuitValue solve(Circuit c);
    public void stop();
}
```
- **solve(Circuit c)** – Starts asynchronous computation of the circuit value and returns a CircuitValue object that allows result retrieval.
- **stop()** – Terminates all ongoing computations and rejects new solve() requests.

## CircuitValue Interface
```java
public interface CircuitValue {
    public boolean getValue() throws InterruptedException;
}
```
- **getValue()** – Blocks the thread until computation completes and returns the result.

## Concurrency
The program allows simultaneous evaluation of multiple circuits and parallel computation of subexpressions within a single expression. The implementation supports interruption handling (stop) and ensures computation correctness in a multi-threaded environment.

## Execution
1. Compile the code using the Java compiler:
   ```sh
   javac *.java
   ```
2. Run the tests to verify program functionality:
   ```sh
   java TestCircuitSolver
   ```

The program represents my implementation of the solution package

