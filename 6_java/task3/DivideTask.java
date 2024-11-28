import java.util.concurrent.RecursiveTask;
import java.util.concurrent.ForkJoinPool;

class DivideTask extends RecursiveTask<int[]> {

    private final int[] arrayToDivide;

    public DivideTask(int[] arrayToDivide) {
        this.arrayToDivide = arrayToDivide;
    }

    @Override
    protected int[] compute() {

        if (arrayToDivide.length <= 1) {
            return (arrayToDivide);
        }

        int mid = arrayToDivide.length / 2;
        int[] leftPart = new int[mid];
        int[] rightPart = new int[arrayToDivide.length - mid];
        System.arraycopy(arrayToDivide, 0, leftPart, 0, mid);
        System.arraycopy(arrayToDivide, mid, rightPart, 0, arrayToDivide.length - mid);

        //tworzenie zadani
        DivideTask task1 = new DivideTask(leftPart);
        DivideTask task2 = new DivideTask(rightPart);

        // podzadania
        task1.fork();
        task2.fork();

        int[] sortedRight = task2.join();
        int[] sortedLeft = task1.join();

        // scalanie posrtowanych
        int[] merged = new int[arrayToDivide.length];
        scal_tab(sortedLeft, sortedRight, merged);
        return merged;
    }

    private void scal_tab(int[] tab1, int[] tab2, int[] scal_tab) {
        int i = 0, j = 0, k = 0;

        while (i < tab1.length && j < tab2.length) {
            if (tab1[i] < tab2[j]) {
                scal_tab[k++] = tab1[i++];
            } else {
                scal_tab[k++] = tab2[j++];
            }
        }

        while (i < tab1.length) {
            scal_tab[k++] = tab1[i++];
        }

        while (j < tab2.length) {
                      scal_tab[k++] = tab2[j++];
        }
    }


}

  
