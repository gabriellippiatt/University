package routes;

import exceptions.TransportFormatException;
import org.junit.Test;
import stops.Stop;
import vehicles.PublicTransport;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.*;

public class RouteTest {
    class TestingRoute extends Route {
        public TestingRoute(String name, int routeNumber) {
            super(name, routeNumber);
        }

        @Override
        public String getType() {
            return "AquaDuck";
        }
    }

    @Test (expected = TransportFormatException.class)
    public void decodeEmptyString() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("", stops);

    }


    @Test (expected = TransportFormatException.class)
    public void decodeNullString() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode(null, stops);

    }

    @Test (expected = TransportFormatException.class)
    public void decodeNullList() throws TransportFormatException{

        TestingRoute.decode("train,red,1: stop0|stop2|stop1", null);
    }

    @Test (expected = TransportFormatException.class)
    public void decodeNoCommas() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("trainred 1: stop0|stop2|stop1", stops);

    }

    @Test (expected = TransportFormatException.class)
    public void decodeOneComma() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("train,red 1: stop0|stop2|stop1", stops);
    }

    @Test (expected = TransportFormatException.class)
    public void decodeIncorrectTransport() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("tram,red 1,:stop0|stop2|stop1", stops);
    }

    @Test (expected = TransportFormatException.class)
    public void decodeMissingColon() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("tram,red, 1stop0|stop2|stop1", stops);
    }

    @Test (expected = TransportFormatException.class)
    public void decodeExtraColon() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("tram,red: 1:stop0|stop2|stop1", stops);
    }

    @Test (expected = TransportFormatException.class)
    public void decodeExtraComma() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);


        TestingRoute.decode("train,red,1,1:stop0|stop2|stop1", stops);
    }

    @Test (expected = TransportFormatException.class)
    public void decodeExtraCommaAtEnd() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("train,red,1:stop0|stop2|stop1,", stops);
    }

    @Test (expected = TransportFormatException.class)
    public void decodeIncorrectType() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("train,red,a:stop0|stop2|stop1,", stops);
    }

    @Test
    public void decodeMissingPipe() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);
        assertEquals("train,red,1:stop0", TestingRoute.decode("train,red,1:stop0", stops).toString());
    }

    @Test (expected = TransportFormatException.class)
    public void decodeMissingParts() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("bus,red,1:||", stops);
    }

    @Test (expected = TransportFormatException.class)
    public void decodeMissingStop() throws TransportFormatException{

        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TestingRoute tRoute = new TestingRoute("red", 4);

        TestingRoute.decode("train,red,1:stop0|stop7|stop1", stops);
    }

    @Test
    public void testDecodeCorrectInput() throws TransportFormatException {
        Stop stop1 = new Stop("stop1", 0, 1);
        Stop stop2 = new Stop("stop2", 1, 2);
        Stop stop3 = new Stop("stop3", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TrainRoute tRoute = new TrainRoute("red", 4);

        System.out.println(TestingRoute.decode("train,red,1:stop3|stop5|stop1", stops).toString());
    }

    @Test
    public void testAddSpaces() throws TransportFormatException {
        Stop stop1 = new Stop(" stop1", 0, 1);
        Stop stop2 = new Stop(" stop2", 1, 2);
        Stop stop3 = new Stop(" stop3", 2, 4);
        Stop stop4 = new Stop(" stop4", 5, 3);
        Stop stop5 = new Stop(" stop5", 4, 2);
        Stop stop6 = new Stop(" stop6", 3, 1);

        ArrayList<Stop> stops = new ArrayList<>();
        stops.add(stop1);
        stops.add(stop2);
        stops.add(stop3);
        stops.add(stop4);
        stops.add(stop5);
        stops.add(stop6);

        TrainRoute tRoute = new TrainRoute("red", 4);

        System.out.println(TestingRoute.decode("train,red, 1 : stop3| stop5| stop1", stops).toString());
    }

    @Test
    public void encodeFerryRoute() {
        Stop stop1 = new Stop("stop1", 0, 1);
        Stop stop2 = new Stop("stop2", 1, 2);
        Stop stop3 = new Stop("stop3", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        FerryRoute fRoute = new FerryRoute("yellow", 3);
        fRoute.addStop(stop4);
        fRoute.addStop(stop5);
        fRoute.addStop(stop6);

        assertEquals("ferry,yellow,3:stop4|stop5|stop6",fRoute.encode());

    }

    @Test
    public void encodeTrainRoute() {
        TrainRoute tRoute = new TrainRoute("red", 1);

        System.out.println(tRoute.encode());

        assertEquals("train,red,1:",tRoute.encode());

    }

    @Test
    public void encodeBusRoute() {
        Stop stop4 = new Stop("stop4", 5, 3);


        BusRoute bRoute = new BusRoute("yellow", 3);
        bRoute.addStop(stop4);

        assertEquals("bus,yellow,3:stop4",bRoute.encode());

    }
}