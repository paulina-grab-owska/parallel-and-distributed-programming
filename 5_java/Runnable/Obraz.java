import  java.util.Random;


class Watek extends Thread {
        private final int znakIndex;
        private final Obraz obraz;
        private final int[] hist_parallel;

        public Watek(int znakIndex, Obraz obraz, int [] hist_parallel){
                this.znakIndex = znakIndex;
                this.obraz = obraz;
                this.hist_parallel = hist_parallel;
        }

        @Override
        public void run() {
                char znak = obraz.getSymbol(znakIndex);
                for (int i = 0; i < obraz.getRows(); i++) {
                        for (int j = 0; j < obraz.getColumns(); j++) {
                                if(obraz.getCharAt(i, j) ==znak) {
                                        synchronized (hist_parallel){
                                                hist_parallel[znakIndex]++;
                                        }
                                }
                        }
                }
        }

}

class Obraz {

    private int size_n;
    private int size_m;
    private char[][] tab;
    private char[] tab_symb;
    private int[] histogram;

    public Obraz(int n, int m) {

        this.size_n = n;
        this.size_m = m;
        tab = new char[n][m];
        tab_symb = new char[94];

        final Random random = new Random();

        // for general case where symbols could be not just integers
        for(int k=0;k<94;k++) {
            tab_symb[k] = (char)(k+33); // substitute symbols
        }

        for(int i=0;i<n;i++) {
            for(int j=0;j<m;j++) {
                tab[i][j] = tab_symb[random.nextInt(94)];  // ascii 33-127
                //tab[i][j] = (char)(random.nextInt(94)+33);  // ascii 33-127
                System.out.print(tab[i][j]+" ");
            }
            System.out.print("\n");
        }
        System.out.print("\n\n");

        histogram = new int[94];
        clear_histogram();
    }

    public void clear_histogram(){

        for(int i=0;i<94;i++) histogram[i]=0;

    }

    public void calculate_histogram(){

          for(int i=0;i<size_n;i++) {
            for(int j=0;j<size_m;j++) {

                // optymalna wersja obliczania histogramu, wykorzystująca fakt, że symbole w tablicy
                // można przekształcić w indeksy tablicy histogramu
                // histogram[(int)tab[i][j]-33]++;

                // wersja bardziej ogólna dla tablicy symboli nie utożsamianych z indeksami
                // tylko dla tej wersji sensowne jest zrównoleglenie w dziedzinie zbioru znaków ASCII
                  for(int k=0;k<94;k++) {
                    if(tab[i][j] == tab_symb[k]) histogram[k]++;
                    //if(tab[i][j] == (char)(k+33)) histogram[k]++;
                  }

            }
          }

    }

     public void print_histogram(){

        for(int i=0;i<94;i++) {
            System.out.print(tab_symb[i]+" "+histogram[i]+"\n");
            //System.out.print((char)(i+33)+" "+histogram[i]+"\n");
        }

    }



    private int[] hist_parallel;

    public char getSymbol(int index) {
        return tab_symb[index];
    }

    public char getCharAt(int i, int j) {
        return tab[i][j];
    }

    public int getRows() {
        return size_n;
    }

    public int getColumns() {
        return size_m;
    }

      public int[] getHistogram(){
        return histogram;
    }


    public int[] calculate_histogram_parallel() {
        hist_parallel = new int[94];
        Watek[] watki = new Watek[94];

        for (int k = 0; k < 94; k++) {
            watki[k] = new Watek(k, this, hist_parallel);
            watki[k].start();
        }

        for (Watek watek : watki) {
            try {
                watek.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        return hist_parallel;
    }

      public void print_parallel_histogram() {
        for (int i = 0; i < 94; i++) {
            System.out.println("Thread " + i + ": " + tab_symb[i] + " = " + hist_parallel[i]);
        }
    }

     public synchronized void printPartialHistogram(int start, int end, int threadId) {
       for (int i = start; i < end; i++) {
           System.out.print("Wątek " + threadId + ": " + tab_symb[i] + " ");
           for (int j = 0; j < histogram[i]; j++) {
               System.out.print("=");
           }
           System.out.println();
       }
   }






}


                    
      
