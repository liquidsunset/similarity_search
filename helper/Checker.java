import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by Daniel Brand on 18.01.17.
 */
public class Checker {

    public static void main(String[] args) {

        if (args.length < 2) {
            System.out.println("Usage: java Checker file1 file2");
            return;
        }

        try {
            ArrayList<Pair<Integer, Integer>> original = createSet(args[0]);
            ArrayList<Pair<Integer, Integer>> test = createSet(args[1]);

            Collections.sort(original);
            Collections.sort(test);

            boolean sim = true;

            if (original.size() == test.size()) {

                for (int i = 0; i < original.size(); i++) {
                    if (!original.get(i).equals(test.get(i))) {
                        sim = false;
                        System.out.println("not sim");
                        break;
                    }
                }

                if (sim) {
                    System.out.println("sim");
                }
            } else {
                System.out.println("not sim");
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    private static ArrayList<Pair<Integer, Integer>> createSet(String fileName) throws FileNotFoundException {

        Scanner scanner = new Scanner(new File(fileName));
        ArrayList<Pair<Integer, Integer>> set = new ArrayList<>();

        while (scanner.hasNextLine()) {
            Pattern p = Pattern.compile("-?\\d+");
            Matcher m = p.matcher(scanner.nextLine());
            ArrayList<Integer> elements = new ArrayList<>();
            while (m.find()) {
                elements.add(Integer.parseInt(m.group()));
            }
            int first = elements.get(0);
            int second = elements.get(1);

            if (first < second) {
                set.add(new Pair<>(elements.get(0), elements.get(1)));
            } else {
                set.add(new Pair<>(elements.get(1), elements.get(0)));
            }
        }
        return set;
    }
}

class Pair<FIRST, SECOND> implements Comparable<Pair<FIRST, SECOND>> {

    private FIRST first;
    private SECOND second;

    Pair(FIRST first, SECOND second) {
        this.first = first;
        this.second = second;
    }

    @Override
    public int compareTo(Pair<FIRST, SECOND> o) {
        int cmp = compare(first, o.first);
        return cmp == 0 ? compare(second, o.second) : cmp;
    }

    // todo move this to a helper class.
    private static int compare(Object o1, Object o2) {
        return o1 == null ? o2 == null ? 0 : -1 : o2 == null ? +1
                : ((Comparable) o1).compareTo(o2);
    }

    @Override
    public int hashCode() {
        return 31 * hashcode(first) + hashcode(second);
    }

    // todo move this to a helper class.
    private static int hashcode(Object o) {
        return o == null ? 0 : o.hashCode();
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Pair))
            return false;
        if (this == obj)
            return true;
        return equal(first, ((Pair) obj).first)
                && equal(second, ((Pair) obj).second);
    }

    // todo move this to a helper class.
    private boolean equal(Object o1, Object o2) {
        return o1 == null ? o2 == null : (o1 == o2 || o1.equals(o2));
    }

    @Override
    public String toString() {
        return "(" + first + ", " + second + ')';
    }
}

