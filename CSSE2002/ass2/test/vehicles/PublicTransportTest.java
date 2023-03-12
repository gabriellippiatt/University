package vehicles;

import exceptions.TransportFormatException;
import org.junit.Test;
import routes.BusRoute;
import routes.FerryRoute;
import routes.Route;
import routes.TrainRoute;
import stops.Stop;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.*;

public class PublicTransportTest {

    class TestingRoute extends Route {
        TestingRoute(String name, int routeNumber) {
            super(name, routeNumber);
        }

        @Override
        public String getType() {
            return "AquaDuck";
        }
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeNullParameters() throws TransportFormatException {
        PublicTransport.decode(null, null);
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeNullParamOne() throws TransportFormatException {
        PublicTransport.decode(null, new ArrayList<>());
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeNullParamTwo() throws TransportFormatException {
        PublicTransport.decode("bus,1,30,1", null);
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeMissingAllCommas() throws TransportFormatException {
        PublicTransport.decode("bus1301", new ArrayList<>());
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeParamOneIncorrectType() throws TransportFormatException {
        PublicTransport.decode("tram,1,30,1", new ArrayList<>());
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeParamOneNAN() throws TransportFormatException {
        PublicTransport.decode("bus,a,30,1", new ArrayList<Route>());
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeMissingTwoCommas() throws TransportFormatException {
        PublicTransport.decode("bus,1301", new ArrayList<Route>());
    }

    @Test (expected = TransportFormatException.class)
    public void testExtraComma() throws TransportFormatException {
        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop6 = new Stop("stop6", 3, 1);


        TrainRoute tRoute = new TrainRoute("red", 4);
        tRoute.addStop(stop1);
        tRoute.addStop(stop2);
        tRoute.addStop(stop3);

        BusRoute bRoute = new BusRoute("blue", 1);
        bRoute.addStop(stop3);
        bRoute.addStop(stop6);

        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(bRoute);
        myRoutes.add(tRoute);

        PublicTransport.decode("train,12,3,30,1,2", myRoutes);
    }

    @Test (expected = TransportFormatException.class)
    public void testInvalidString() throws TransportFormatException {
        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop6 = new Stop("stop6", 3, 1);


        TrainRoute tRoute = new TrainRoute("red", 2);
        tRoute.addStop(stop1);
        tRoute.addStop(stop2);
        tRoute.addStop(stop3);

        BusRoute bRoute = new BusRoute("blue", 1);
        bRoute.addStop(stop3);
        bRoute.addStop(stop6);

        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(bRoute);
        myRoutes.add(tRoute);

        PublicTransport.decode("train,123,30,2,2,", myRoutes);
    }

    @Test (expected = TransportFormatException.class)
    public void testExtraValues() throws TransportFormatException {
        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop6 = new Stop("stop6", 3, 1);


        TrainRoute tRoute = new TrainRoute("red", 2);
        tRoute.addStop(stop1);
        tRoute.addStop(stop2);
        tRoute.addStop(stop3);

        BusRoute bRoute = new BusRoute("blue", 1);
        bRoute.addStop(stop3);
        bRoute.addStop(stop6);

        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(bRoute);
        myRoutes.add(tRoute);

        PublicTransport.decode("train,123,30,2,2,8,9", myRoutes);
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeParamOneNAN2() throws TransportFormatException {
        PublicTransport.decode("bus,1,b,1", new ArrayList<>());
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeMissingOneComma() throws TransportFormatException {
        PublicTransport.decode("bus,1,301", new ArrayList<>());
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeParamOneNAN3() throws TransportFormatException {
        PublicTransport.decode("bus,1,30,c", new ArrayList<>());
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeRouteNotInList() throws TransportFormatException {
        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(new TestingRoute("bus", 2));
        PublicTransport.decode("bus,1,30,1", myRoutes);
    }

    @Test(expected = TransportFormatException.class)
    public void testDecodeRouteNotInList2() throws TransportFormatException {
        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(new TestingRoute("bus", 2));
        myRoutes.add(new TestingRoute("", 3));
        myRoutes.add(new TestingRoute("", 5));
        myRoutes.add(new TestingRoute("", 7));
        PublicTransport.decode("bus,1,30,1", myRoutes);
    }

    @Test (expected = TransportFormatException.class)
    public void testInvalidVehicle() throws TransportFormatException {
        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop6 = new Stop("stop6", 3, 1);

        TrainRoute tRoute = new TrainRoute("red", 4);
        tRoute.addStop(stop1);
        tRoute.addStop(stop2);
        tRoute.addStop(stop3);

        BusRoute bRoute = new BusRoute("blue", 1);
        bRoute.addStop(stop3);
        bRoute.addStop(stop6);

        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(bRoute);
        myRoutes.add(tRoute);

        //bus,412,20,2,ABC123
        Bus bus = new Bus(123, 20, bRoute, "AAA111");

        PublicTransport transport = PublicTransport.decode("tram,123,20,1,ABC123", myRoutes);
    }

    @Test (expected = TransportFormatException.class)
    public void testInvalidRouteNum() throws TransportFormatException {
        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop6 = new Stop("stop6", 3, 1);

        TrainRoute tRoute = new TrainRoute("red", 4);
        tRoute.addStop(stop1);
        tRoute.addStop(stop2);
        tRoute.addStop(stop3);

        BusRoute bRoute = new BusRoute("blue", 1);
        bRoute.addStop(stop3);
        bRoute.addStop(stop6);

        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(bRoute);
        myRoutes.add(tRoute);

        //bus,412,20,2,ABC123
        Bus bus = new Bus(123, 20, bRoute, "AAA111");

        PublicTransport transport = PublicTransport.decode("train,123,20,1,ABC123", myRoutes);
    }

    @Test
    public void testDecodeRouteNumSingle() throws TransportFormatException {
        Stop stop1 = new Stop("stop0", 0, 1);
        Stop stop2 = new Stop("stop1", 1, 2);
        Stop stop3 = new Stop("stop2", 2, 4);
        Stop stop6 = new Stop("stop6", 3, 1);

        TrainRoute tRoute = new TrainRoute("red", 4);
        tRoute.addStop(stop1);
        tRoute.addStop(stop2);
        tRoute.addStop(stop3);

        BusRoute bRoute = new BusRoute("blue", 1);
        bRoute.addStop(stop3);
        bRoute.addStop(stop6);

        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(bRoute);
        myRoutes.add(tRoute);

        //bus,412,20,2,ABC123
        Bus bus = new Bus(123, 20, bRoute, "AAA111");

        PublicTransport transport = PublicTransport.decode("bus,123,20,1,ABC123", myRoutes);

        assertEquals(bus.toString(), transport.toString());
    }

    @Test
    public void testDecodeRouteNumSecond() throws TransportFormatException {
        Stop stop1 = new Stop("stop1", 0, 1);
        Stop stop2 = new Stop("stop2", 1, 2);
        Stop stop3 = new Stop("stop3", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        //bus,blue,2:stop1|stop3|stop0

        TrainRoute tRoute = new TrainRoute("red", 1);
        tRoute.addStop(stop1);
        tRoute.addStop(stop2);
        tRoute.addStop(stop3);

        BusRoute bRoute = new BusRoute("blue", 2);
        bRoute.addStop(stop3);
        bRoute.addStop(stop6);

        FerryRoute fRoute = new FerryRoute("yellow", 3);
        fRoute.addStop(stop4);
        fRoute.addStop(stop5);
        fRoute.addStop(stop6);


        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(tRoute);
        myRoutes.add(bRoute);
        myRoutes.add(fRoute);

        //bus,412,20,2,ABC123
        Bus bus = new Bus(123, 20, bRoute, "AAA111");
        // spaces in string
        PublicTransport transport = PublicTransport.decode("bus, 123,20,2, ABC123", myRoutes);

        assertEquals( bus.toString(), transport.toString());
    }


    @Test
    public void testDecodeRouteNumThird() throws TransportFormatException {
        Stop stop1 = new Stop("stop1", 0, 1);
        Stop stop2 = new Stop("stop2", 1, 2);
        Stop stop3 = new Stop("stop3", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        //bus,blue,2:stop1|stop3|stop0

        TrainRoute tRoute = new TrainRoute("red", 1);
        tRoute.addStop(stop1);
        tRoute.addStop(stop2);
        tRoute.addStop(stop3);

        BusRoute bRoute = new BusRoute("blue", 2);
        bRoute.addStop(stop3);
        bRoute.addStop(stop6);

        FerryRoute fRoute = new FerryRoute("yellow", 3);
        fRoute.addStop(stop4);
        fRoute.addStop(stop5);
        fRoute.addStop(stop6);

        ArrayList<Route> myRoutes = new ArrayList<>();
        myRoutes.add(tRoute);
        myRoutes.add(bRoute);
        myRoutes.add(fRoute);

        //bus,412,20,2,ABC123
        Ferry ferry = new Ferry(123, 20, fRoute, "hydro");

        PublicTransport transport = PublicTransport.decode("ferry, 123 , 20 , 3 ,hydro", myRoutes);

        assertEquals( ferry.toString(), transport.toString());
    }

    @Test (expected = TransportFormatException.class)
    public void testDecodeWithEmptyRoute() throws TransportFormatException {
        List<Route> routes = new ArrayList<>();
        routes.add(new TestingRoute("City", 412));
        PublicTransport.decode("AquaDuck,500,30,412", routes);
    }

    @Test
    public void encodeFerry() {
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

        Ferry ferry = new Ferry(123, 20, fRoute, "hydro");
        assertEquals("ferry,123,20,3,hydro", ferry.encode());
    }

    @Test
    public void encodeBus(){
        Stop stop1 = new Stop("stop1", 0, 1);
        Stop stop2 = new Stop("stop2", 1, 2);
        Stop stop3 = new Stop("stop3", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        BusRoute bRoute = new BusRoute("blue", 2);
        bRoute.addStop(stop3);
        bRoute.addStop(stop6);

        Bus bus = new Bus(123, 20, bRoute, "AAA111");
        assertEquals("bus,123,20,2,AAA111",bus.encode());
    }

    @Test
    public void encodeTrain() {
        Stop stop1 = new Stop("stop1", 0, 1);
        Stop stop2 = new Stop("stop2", 1, 2);
        Stop stop3 = new Stop("stop3", 2, 4);
        Stop stop4 = new Stop("stop4", 5, 3);
        Stop stop5 = new Stop("stop5", 4, 2);
        Stop stop6 = new Stop("stop6", 3, 1);

        TrainRoute tRoute = new TrainRoute("red", 1);
        tRoute.addStop(stop1);
        tRoute.addStop(stop2);
        tRoute.addStop(stop3);

        Train train = new Train(5, 100, tRoute, 6);
        assertEquals("train,5,100,1,6", train.encode());
    }
}
