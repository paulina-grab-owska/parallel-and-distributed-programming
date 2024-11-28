import java.util.concurrent.RecursiveTask;
import java.util.concurrent.ForkJoinPool;

public class SortWithForkJoin {
    public static void main(String[] args) {

        int[] array = {38, 27, 43, 3, 9, 82, 10, 7, 55, 1, 67, 5};

        System.out.println("Not sorted array: ");
        for (int num : array) {
                System.out.print(num + " ");
        }
        System.out.println();

        ForkJoinPool pool = new ForkJoinPool();

        DivideTask mainTask = new DivideTask(array);
        int[] sortedArray = pool.invoke(mainTask);

        System.out.println("Sorted array:");
        for (int num : sortedArray) {
            System.out.print(num + " ");
        }
        System.out.println();
    }
}
