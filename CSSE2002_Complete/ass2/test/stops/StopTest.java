package stops;

import exceptions.TransportFormatException;
import org.junit.Test;

import static org.junit.Assert.*;

public class StopTest {

    @Test(expected = TransportFormatException.class)
    public void decodeEmptyString() throws TransportFormatException{
        Stop.decode("");
    }

    @Test(expected = TransportFormatException.class)
    public void decodeNullString() throws TransportFormatException{
        Stop.decode(null);
    }

    @Test(expected = TransportFormatException.class)
    public void decodeMissingStopName() throws TransportFormatException{
        Stop.decode(":0:1");
    }

    @Test(expected = TransportFormatException.class)
    public void decodeIncorrectX() throws TransportFormatException{
        Stop.decode("stop1:a:1");
    }

    @Test(expected = TransportFormatException.class)
    public void decodeIncorrectY() throws TransportFormatException{
        Stop.decode("stop1:2:y");
    }

    @Test(expected = TransportFormatException.class)
    public void decodeExtraDelimiter() throws TransportFormatException{
        Stop.decode("stop1:3:1:");
    }

    @Test(expected = TransportFormatException.class)
    public void decodeExtraDelimiter2() throws TransportFormatException{
        Stop.decode("stop1:3:1:4");
    }

    @Test
    public void decodeSpaceAroundNumber() throws TransportFormatException{
        assertEquals("stop1:3:1", Stop.decode("stop1: 3 : 1 ").toString());
    }

    @Test
    public void decodeSpaceAroundString() throws TransportFormatException{
        assertEquals(" stop1 :3:1", Stop.decode(" stop1 :3: 1 ").toString());
    }

    @Test
    public void decodeValidStop() throws TransportFormatException{
        assertEquals("stop1:3:1", Stop.decode("stop1:3:1").toString());
    }

    @Test
    public void encodeStop() {
        Stop stop1 = new Stop("stop1", 3, 1);
        assertEquals("stop1:3:1", stop1.encode());
    }
}
