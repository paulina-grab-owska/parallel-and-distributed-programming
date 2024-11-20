import java.util.Scanner;
import java.lang.Thread;

class Watek extends Thread {
        int il;
        int at;
        Obraz obraz;

        public Watek(int il, int at, Obraz obraz){
                this.il = il;
                this.at = at;
                this.obraz = obraz;
        }

        @Override
        public void run() {
                int e = 94;
                int start = (int)((double)e*((double)at/(double)il));
                int end = (int)((double)e*((double)(1+at)/(double)il));
        t(start, end, at);
}

        public void t (int start, int end, int at) {
                for (int i = start; i<end; i++) {
                        obraz.print_histogram(i, at);
}

}

}
class Histogram_test {

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        System.out.println("Set image size: n (#rows), m(#kolumns)");
        int n = scanner.nextInt();
        int m = scanner.nextInt();
        Obraz obraz_1 = new Obraz(n, m);

//-----wairant 1

        obraz_1.calculate_histogram();
        //obraz_1.print_histogram();
        obraz_1.clear_histogram();

        System.out.println("Set number of threads");
        int num_threads = scanner.nextInt();

        Watek[] NewThr = new Watek[num_threads];

        for (int i = 0; i < num_threads; i++) {
            (NewThr[i] = new Watek(num_threads,i,obraz_1)).start();
         }

        for (int i = 0; i < num_threads; i++) {
                  for (int i = 0; i < num_threads; i++) {
            try {
                NewThr[i].join();
            } catch (InterruptedException e) {}
         }

        System.out.println("\nHistogram calculation completed.");




  }

}
