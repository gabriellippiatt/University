package exceptions;

import stops.Stop;
import network.Network;
/**
 * Exception thrown when a duplicate {@link Stop} (as defined by Stop
 * .equals(Object)) is added to a {@link Network}.
 *
 */
public class DuplicateStopException extends TransportException {
}
