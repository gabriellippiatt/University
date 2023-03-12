package network;

import exceptions.TransportFormatException;
import org.junit.Test;
import routes.Route;
import stops.Stop;
import vehicles.PublicTransport;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.*;

public class NetworkTest {

    private static final String NEWLINE = System.lineSeparator();

    class TestingRoute extends Route {
        TestingRoute(String name, int routeNumber) {
            super(name, routeNumber);
        }

        @Override
        public String getType() {
            return "AquaDuck";
        }
    }

    class TestingTransport extends PublicTransport {
        TestingTransport(int id, int capacity, Route route) {
            super(id, capacity, route);
        }
    }

    @Test
    public void testEmptyConstructor() {
        Network network = new Network();
        assertTrue(network.getRoutes().size() == network.getStops().size() &&
                network.getVehicles().size() == network.getRoutes().size() &&
                network.getStops().size() == 0);
    }


    @Test (expected = java.io.IOException.class)
    public void testNullFilename() throws IOException, TransportFormatException{
        Network network = new Network(null);
    }

    @Test (expected = java.io.IOException.class)
    public void testFileDoesNotExist() throws IOException, TransportFormatException{
        Network network = new Network("abc.txt");
    }

    @Test
    public void testInvalidBlankLines() {
        try {
            Network network = new Network("maps/invalidBlankLines.txt");
            fail();
        } catch (IOException e) {
            fail();
        } catch (TransportFormatException e) {
            // pass
        }
    }

    @Test
    public void testInvalidBlankLinesEndOfFile() {
        try {
            Network network = new Network("maps/invalidBlankLinesEndOfFile.txt");
            fail();
        } catch (IOException e) {
            fail();
        } catch (TransportFormatException e) {
            // pass
        }
    }

    @Test
    public void testInvalidRouteCount() {
        try {
            Network network = new Network("maps/invalidRouteCount.txt");
            fail();
        } catch (IOException e) {
            fail();
        } catch (TransportFormatException e) {
            // pass
        }
    }

    @Test
    public void testInvalidStopCount() {
        try {
            Network network = new Network("maps/invalidStopCount.txt");
            fail();
        } catch (IOException e) {
            fail();
        } catch (TransportFormatException e) {
            // pass
        }
    }

    @Test
    public void testInvalidTransportCount() {
        try {
            Network network = new Network("maps/invalidTransportCount.txt");
            fail();
        } catch (IOException e) {
            fail();
        } catch (TransportFormatException e) {
            // pass
        }
    }

    @Test
    public void testValidFromSpec() {
        try {
            Network network = new Network("maps/validFromSpec.txt");
            // pass
        } catch (IOException | TransportFormatException e) {
            fail();
        }
    }

    @Test
    public void testAddNullStop() {
        Network network = new Network();
        network.addStop(null);
        assertFalse(network.getStops().contains(null));
        assertEquals(0, network.getStops().size());
    }

    @Test
    public void testAddStop() {
        Network network = new Network();
        Stop stop = new Stop("CollaborateAndListen", 4, 2);
        network.addStop(stop);

        assertEquals(1, network.getStops().size());
        assertTrue(network.getStops().contains(stop));
    }

    @Test
    public void testAddNullStopInList() {
        List<Stop> stops = new ArrayList<>();
        stops.add(new Stop("CollaborateAndListen", 4, 2));
        stops.add(null);
        stops.add(new Stop("HammerTime", 0, 0));

        Network network = new Network();
        network.addStops(stops);

        assertEquals(0, network.getStops().size());
    }

    @Test
    public void testAddStops() {
        List<Stop> stops = new ArrayList<>();
        Stop first = new Stop("CollaborateAndListen", 4, 2);
        stops.add(first);
        Stop second = new Stop("HammerTime", 0, 0);
        stops.add(second);

        Network network = new Network();
        network.addStops(stops);

        assertEquals(2, network.getStops().size());
        assertTrue(network.getStops().contains(first));
        assertTrue(network.getStops().contains(second));
    }

    @Test
    public void testGetStopsUnmodifiable() {
        Network network = new Network();
        List<Stop> returned = network.getStops();
        returned.add(null);

        assertEquals(0, network.getStops().size());
    }

    @Test
    public void testAddNullRoute() {
        Network network = new Network();
        network.addRoute(null);
        assertEquals(0, network.getRoutes().size());
    }

    @Test
    public void testAddRoute() {
        Network network = new Network();
        Route route = new TestingRoute("City", 412);
        network.addRoute(route);

        assertEquals(1, network.getRoutes().size());
        assertTrue(network.getRoutes().contains(route));
    }

    @Test
    public void testGetRoutesUnmodifiable() {
        Network network = new Network();
        List<Route> returned = network.getRoutes();
        returned.add(null);

        assertEquals(0, network.getRoutes().size());
    }

    @Test
    public void testAddNullVehicle() {
        Network network = new Network();
        network.addVehicle(null);
        assertEquals(0, network.getVehicles().size());
    }

    @Test
    public void testAddVehicle() {
        Network network = new Network();
        PublicTransport transport = new TestingTransport(4, 2, new TestingRoute("City", 412));
        network.addVehicle(transport);

        assertEquals(1, network.getVehicles().size());
        assertTrue(equalVehicles(transport, network.getVehicles().get(0)));
    }

    private boolean equalVehicles(PublicTransport first, PublicTransport second) {
        return first.getType().equals(second.getType())
                && first.getId() == second.getId()
                && first.getCapacity() == second.getCapacity()
                && first.getRoute().equals(second.getRoute());
    }

    @Test
    public void testGetVehiclesUnmodifiable() {
        Network network = new Network();
        List<PublicTransport> removed = network.getVehicles();
        removed.add(null);

        assertEquals(0, network.getVehicles().size());
    }

    @Test
    public void testSaveNull() throws IOException {
        Network network = new Network();
        network.save(null); // shouldn't throw any exceptions
    }

    @Test
    public void testSaveBasic() throws IOException {
        Network network = new Network();
        network.save("saveBasic.txt");

        String expected = "0" + NEWLINE
                + "0" + NEWLINE
                + "0" + NEWLINE;

        BufferedReader reader = new BufferedReader(new FileReader("saveBasic.txt"));
        StringBuilder actual = new StringBuilder();
        String line;
        while ((line = reader.readLine()) != null) {
            actual.append(line).append(NEWLINE);
        }

        assertEquals(expected, actual.toString());
    }

    @Test
    public void testSave() throws IOException {
        Network network = new Network();

        Stop firstStop = new Stop("CollaborateAndListen", 4, 2);
        network.addStop(firstStop);
        Stop secondStop = new Stop("HammerTime", 0, 0);
        network.addStop(secondStop);

        Route route = new TestingRoute("City", 412);
        route.addStop(firstStop);
        route.addStop(secondStop);
        network.addRoute(route);

        PublicTransport transport = new TestingTransport(500, 30, route);
        network.addVehicle(transport);

        network.save("saved.txt");

        String expected = "2" + NEWLINE
                + "CollaborateAndListen:4:2" + NEWLINE
                + "HammerTime:0:0" + NEWLINE
                + "1" + NEWLINE
                + "AquaDuck,City,412:CollaborateAndListen|HammerTime" + NEWLINE
                + "1" + NEWLINE
                + "AquaDuck,500,30,412" + NEWLINE;

        BufferedReader reader = new BufferedReader(new FileReader("saved.txt"));
        StringBuilder actual = new StringBuilder();
        String line;
        while ((line = reader.readLine()) != null) {
            actual.append(line).append(NEWLINE);
        }

        assertEquals(expected, actual.toString());
    }
}
