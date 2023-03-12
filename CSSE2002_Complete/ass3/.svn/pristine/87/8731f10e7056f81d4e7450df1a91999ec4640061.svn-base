package stops;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.util.List;


public class RoutingTableTest {

    @Before
    public void setUp() throws Exception {
    }

    @After
    public void tearDown() throws Exception {
    }

    @Test
    public void addNeighbour() {
        Stop A = new Stop("A",0,4);
        Stop B = new Stop("B",0,2);
        A.getRoutingTable().addNeighbour(B);
        A.addNeighbouringStop(B);
        Assert.assertTrue(A.getNeighbours().contains(B));
        Assert.assertTrue(A.getRoutingTable().getCosts().keySet().contains(B));
    }

    @Test
    public void addOrUpdateEntry() {
        Stop A = new Stop("A",0,4);
        Stop B = new Stop("B",0,2);
        Stop C = new Stop("C",0,0);

        A.getRoutingTable().addOrUpdateEntry(B,5,C);
        Assert.assertTrue(A.getRoutingTable().getCosts().keySet().contains(B));
    }

    @Test
    public void costTo() {
        Stop A = new Stop("A",0,4);
        Stop B = new Stop("B",0,2);

        A.addNeighbouringStop(B);

        int actual = A.distanceTo(B);
        int expected = A.getRoutingTable().costTo(B);

        Assert.assertEquals(expected,actual);
    }

    @Test
    public void getCosts() {
        Stop A = new Stop("A",0,4);
        Stop B = new Stop("B",0,2);

        A.addNeighbouringStop(B);

        Assert.assertTrue(A.getRoutingTable().getCosts().keySet().contains(B));
    }

    @Test
    public void getStop() {
        Stop A = new Stop("A",0,4);
        Stop B = new Stop("B",0,2);

        Assert.assertEquals(A.getRoutingTable().getStop(),A);
        Assert.assertEquals(B.getRoutingTable().getStop(),B);
    }

    @Test
    public void nextStop() {
        Stop A = new Stop("A",0,4);
        Stop B = new Stop("B",0,2);

        A.addNeighbouringStop(B);

        Assert.assertEquals(A.getRoutingTable().nextStop(B), B);
    }

    @Test
    public void synchronise() {
        // create stops
        Stop A = new Stop("A",0,4);
        Stop B = new Stop("B",0,2);
        Stop C = new Stop("C",0,0);
        Stop D = new Stop("D",2,2);

        // populate Routing Table for A
        //A.addNeighbouringStop(B);--> removing this to see if it will be added
        A.addNeighbouringStop(D);
        A.getRoutingTable().addOrUpdateEntry(C,
                A.distanceTo(B)+B.distanceTo(C),B);

        // populate Routing Table for B
        B.addNeighbouringStop(A);
        B.addNeighbouringStop(D);
        B.addNeighbouringStop(C);

        // populate Routing Table for C
        C.addNeighbouringStop(B);
        C.addNeighbouringStop(D);
        // here C takes an inefficient route via D (could save cost going
        //                                                        via B instead)
        C.getRoutingTable().addOrUpdateEntry(A,
                A.distanceTo(D)+D.distanceTo(C),D);

        // populate Routing Table for D
        D.addNeighbouringStop(A);
        D.addNeighbouringStop(B);
        D.addNeighbouringStop(C);

        A.getRoutingTable().synchronise();

        int actual = A.getRoutingTable().getCosts().get(B);

        Assert.assertEquals(6,actual);

        Assert.assertEquals(B, C.getRoutingTable().nextStop(A));
    }

    @Test
    public void transferEntries() {
        // create stops
        Stop A = new Stop("A",0,4);
        Stop B = new Stop("B",0,2);
        Stop C = new Stop("C",0,0);
        Stop D = new Stop("D",2,2);

        // populate Routing Table for A
        A.addNeighbouringStop(B);
        A.addNeighbouringStop(D);
        A.getRoutingTable().addOrUpdateEntry(C,
                A.distanceTo(B)+B.distanceTo(C),B);

        // populate Routing Table for B
        B.addNeighbouringStop(A);
        B.addNeighbouringStop(D);
        B.addNeighbouringStop(C);

        // populate Routing Table for C
        C.addNeighbouringStop(B);
        C.addNeighbouringStop(D);
        // here C takes an inefficient route via D (could save cost going
        //                                                        via B instead)
        C.getRoutingTable().addOrUpdateEntry(A,
                C.distanceTo(D)+D.distanceTo(A),D);

        // populate Routing Table for D
        D.addNeighbouringStop(A);
        D.addNeighbouringStop(B);
        D.addNeighbouringStop(C);

        // Check if the transfer was complete
        Assert.assertEquals(B, C.getRoutingTable().nextStop(A));

        int expected = C.distanceTo(A);
        int actual = C.getRoutingTable().getCosts().get(A);

        Assert.assertEquals(expected, actual);
    }

    @Test
    public void traverseNetwork() {
        // create stops
        Stop A = new Stop("A",0,4);
        Stop B = new Stop("B",0,2);
        Stop C = new Stop("C",0,0);
        Stop D = new Stop("D",2,2);

        // populate Routing Table for A
        A.addNeighbouringStop(B);
        A.addNeighbouringStop(D);
        A.getRoutingTable().addOrUpdateEntry(C,
                A.distanceTo(B)+B.distanceTo(C),B);

        // populate Routing Table for B
        B.addNeighbouringStop(A);
        B.addNeighbouringStop(D);
        B.addNeighbouringStop(C);

        // populate Routing Table for C
        C.addNeighbouringStop(B);
        C.addNeighbouringStop(D);
        C.getRoutingTable().addOrUpdateEntry(A,
                A.distanceTo(B)+B.distanceTo(A),B);

        // populate Routing Table for D
        D.addNeighbouringStop(A);
        D.addNeighbouringStop(B);
        D.addNeighbouringStop(C);

        // the traverse network list
        List<Stop> traverseList = A.getRoutingTable().traverseNetwork();

        Assert.assertTrue(traverseList.contains(A));
        Assert.assertTrue(traverseList.contains(B));
        Assert.assertTrue(traverseList.contains(C));
        Assert.assertTrue(traverseList.contains(D));
    }
}