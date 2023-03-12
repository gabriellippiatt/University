package exceptions;

import network.Network;

/**
 * Exception thrown when an encoded {@link Network} file is
 * formatted incorrectly.
 *
 * (i.e.
 * 1- If any of the lines representing stops, routes, or vehicles are
 *    incorrectly formatted according to their
 *    respective decode methods (i.e. if their decode method throws an
 *    exception).
 * 2- If any of the integers are incorrectly formatted (i.e. cannot be parsed).
 * 3- If the {number_of_stops} does not match the actual number of lines
 *    representing stops present. This also applies
 *    to {number_of_routes} and {number_of_vehicles}.
 * 4- If there are any extra lines present in the file (the file may end with
 *    a  single newline character, but there may
 *    not be multiple blank lines at the end of the file).
 * 5- If any other formatting issues are encountered whilst parsing the file.)
 */
public class TransportFormatException extends TransportException {
}
