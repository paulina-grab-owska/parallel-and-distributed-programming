import java.util.Scanner;

public class Main {
    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        System.out.print("Podaj początek przedziału (xp): ");
        double xp = scanner.nextDouble();

        System.out.print("Podaj koniec przedziału (xk): ");
        double xk = scanner.nextDouble();

        System.out.print("Podaj dokładność (dx): ");
        double dx = scanner.nextDouble();

        //konstruktor
        Calka_callable calka = new Calka_callable(xp, xk, dx);

        double wynik = calka.compute_integral();

        System.out.println("Przybliżona wartość całki wynosi: " + wynik);

        scanner.close();
    }
}

