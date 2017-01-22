import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Stream;
import java.util.stream.Collectors;
import java.util.List;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.io.PrintWriter;

// This tool scans a text file, and ensures that there are no duplicate words
// PER LINE. Delimiter: whitespace.
// Usage: java UnDuplicate <input-file> <output-file>

public class UnDuplicate {
  public static void main(String[] args) throws IOException {

  	if (args.length != 2) {
  		System.out.println("Usage: java UnDuplicate <input-file> <output-file>");
  		return;
  	}
		String file = args[0];
		List<String> words;
		Set<String> uniqueWords;
    PrintWriter writer = new PrintWriter(args[1], "UTF-8");

		try (Stream<String> stream = Files.lines(Paths.get(file))) {
			List<String> lines = stream.collect(Collectors.toList());
			for(int i = 0; i < lines.size(); i++) {
				words = Arrays.asList(lines.get(i).split("\\s+"));
 				uniqueWords = new HashSet<String>(words);

        for (int j = 0; j < words.size(); j++) {
            if (Collections.frequency(words, words.get(j)) > 1) {
            	words.set(j,words.get(j)+j);
            }
           writer.print(words.get(j) + " ");
        }
        writer.println();
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			writer.close();
			System.out.println("Finished.");
		}
  }
}