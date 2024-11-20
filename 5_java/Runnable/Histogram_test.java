import java.util.Scanner;


class Histogram_test {

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        System.out.println("Set image size: n (#rows), m(#kolumns)");
        int n = scanner.nextInt();
        int m = scanner.nextInt();
        Obraz obraz_1 = new Obraz(n, m);

   System.out.println("Set number of threads");
        int numThreads = scanner.nextInt();

        int blockSize = 94 / numThreads;
        Thread[] threads = new Thread[numThreads];

        for (int i = 0; i < numThreads; i++) {
            int startIndex = i * blockSize;
            int endIndex = (i == numThreads - 1) ? 94 : startIndex + blockSize;
            threads[i] = new Thread(new RunnableTask(obraz_1, startIndex, endIndex, i + 1));
            threads[i].start();
        }

        for (int i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

      
        System.out.println("\nHistogram calculation completed.");




  }

}

