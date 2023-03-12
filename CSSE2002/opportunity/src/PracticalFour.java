import java.io.FileNotFoundException;
import java.util.Random;
public class PracticalFour {
    private static void makeException(int num) throws FileNotFoundException, UnknownExceptionException {
        switch (num) {
            case 0:
                throw new NullPointerException();
            case 1:
                throw new ArrayIndexOutOfBoundsException();
            case 2:
                System.out.println("No Problems");
                break;
            case 4:
                throw new FileNotFoundException();
            default:
                throw new UnknownExceptionException();
        }
    }

    private static void f() throws FileNotFoundException, UnknownExceptionException {
        try {
            makeException(0);
        } catch(NullPointerException e) {
        }
        try {
            makeException(2);
            makeException(4);
            makeException(2);
        } catch (Exception e) {
            System.out.println(e);
        }
    }
    //example for squashing
    private void squash(int num) {
        try {
            makeException(num);
        } catch (Exception e ) {
        }
    }

    public static void main(String[] args) throws FileNotFoundException, UnknownExceptionException {
        f();

/*
Random random = new Random();
for (int i = 0; i < 5; i++) {
h(random);
}
*/
    }
}
