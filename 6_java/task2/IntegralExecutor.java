import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class IntegralExecutor {

    public static void main(String[] args) {

        double xp = 0.0;
        double xk = Math.PI;
        double dx = 0.001;
        int numThreads = 10;          // liczb wątkow
        int numTasks = 40;           // liczba zadań

        // szeokosc kazdego podprzedialu
        double subIntervalWidth = (xk - xp) / numTasks;

        // pula watków
        ExecutorService executor = Executors.newFixedThreadPool(numThreads);

        // lista Future przechwująca wyniki
        List<Future<Double>> results = new ArrayList<>();

        // tworzenie zadan
        for (int i = 0; i < numTasks; i++) {
            double subXp = xp + i * subIntervalWidth;
            double subXk = subXp + subIntervalWidth;

                  Calka_callable task = new Calka_callable(subXp, subXk, dx);
            Future<Double> result = executor.submit(task);

            results.add(result);
        }

        // zamknij pule
        executor.shutdown();

        double totalIntegral = 0.0;
        for (Future<Double> future : results) {
            try {
                totalIntegral += future.get();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        System.out.println("Final integral value: " + totalIntegral);
    }
}
