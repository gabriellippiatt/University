package utilities;

/**
 * Defines the interface for classes which are able to be encoded for writing
 * into files.
 * Creates a string representation of the object for writing to a file.
 *
 * @return A string representation of the object.
 */

public interface Writeable {
    String encode();
}
