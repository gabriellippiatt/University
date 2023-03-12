package network;

import exceptions.TransportFormatException;
import routes.Route;
import stops.Stop;
import vehicles.PublicTransport;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class Network {
    // The route to add to the network.
    private Route route;

    // The stop to add to the network.
    private Stop stop;

    /**
     * Creates a new empty Network with no stops, vehicles, or routes.
     */

    // A list of stops in the transport network
    private List<Stop> stops = new ArrayList<>();

    // A list of routes in the transport network
    private List<Route> routes = new ArrayList<>();

    // A list of vehicles in the transport network
    private List<PublicTransport> vehicles = new ArrayList<>();


    /**
     * Creates a new Network from information contained in the file indicated
     * by the given filename.
     * The file should be in the following format:
     *
     * <p>{number_of_stops}
     * {stop0:x0:y0}
     * ...
     * {stopN:xN:yN}
     * {number_of_routes}
     * {type0,name0,number0:stop0|stop1|...|stopM}
     * ...
     * {typeN,nameN,numberN:stop0|stop1|...|stopM}
     * {number_of_vehicles}
     * {type0,id0,capacity0,routeNumber,extra}
     * ...
     * {typeN,idN,capacityN,routeNumber,extra}
     * where {number_of_stops}, {number_of_routes}, and {number_of_vehicles}
     * are the number of stops, routes, and vehicles (respectively) in the
     * network, and where {stop0,x0,y0} is the encode() representation of a
     * Stop, {type0,name0,number0:stop0|stop1|...|stopM} is the encode()
     * representation of a Route, and {typeN,idN,capacityN,routeNumber,extra}
     * is the encode() representation of a PublicTransport.
     *
     * <p>Whilst parsing, if spaces (i.e. ' ') are encountered before or after
     * integers, (i.e. {number_of_stops}, {number_of_routes}, or
     * {number_of_vehicles}), the spaces should simply be trimmed (for example,
     * using something like String.trim()).
     *
     * For example:
     *
     * <p>4
     * stop0:0:1
     * stop1:-1:0
     * stop2:4:2
     * stop3:2:-8
     * 2
     * train,red,1:stop0|stop2|stop1
     * bus,blue,2:stop1|stop3|stop0
     * 3
     * train,123,30,1,2
     * train,42,60,1,3
     * bus,412,20,2,ABC123
     *
     * The Network object created should have the stops, routes, and vehicles
     * contained in the given file.
     *
     * @param filename The name of the file to load the network from.
     * @throws IOException If any IO exceptions occur whilst trying to read
     *                          from the file, or if the filename is null.
     * @throws TransportFormatException
     * 1 - If any of the lines representing stops, routes, or vehicles are
     *     incorrectly formatted according to their respective decode methods
     *     (i.e. if their decode method throws an exception).
     * 2 - If any of the integers are incorrectly formatted (i.e. cannot be
     *     parsed).
     * 3 - If the {number_of_stops} does not match the actual number of lines
     *     representing stops present. This also applies to {number_of_routes}
     *     and {number_of_vehicles}. An error should also be thrown if any of
     *     these integers are negative.
     * 4 - If there are any extra lines present in the file (the file may end
     *     with a single newline character, but there may not be multiple blank
     *     lines at the end of the file).
     * 5 - If any other formatting issues are encountered whilst parsing the
     *     file (sample valid and invalid network files will be provided to
     *     help identify some potential issues).
     */
    public Network(String filename)
            throws IOException, TransportFormatException {

        if (filename == null) {
            throw new IOException();
        }


        String line = (new BufferedReader(new FileReader(filename))).readLine();
        String[] splits = line.split(":",3);
        Stop stop = new Stop(splits[0], Integer.parseInt(splits[1]), Integer.parseInt(splits[2]));
    }

    /**
     * public void addRoute(Route route)
     * Adds the given route to the network.
     * If the given route is null, it should not be added to the network.
     *
     * Parameters:
     * route - The route to add to the network.
     * @param route The route to add to the network.
     */
    public void addRoute(Route route) {
        if (route == null) {
            return;
        }

        routes.add(route);
    }

    /**
     * Adds the given stop to the transportation network.
     * If the given stop is null, it should not be added to the network.
     *
     * @param stop The stop to add to the network.
     */
    public void addStop(Stop stop) {
        if (stop == null) {
            return;
        }

        stops.add(stop);
    }

    /**
     * Adds multiple stops to the transport network.
     * If any of the stops in the given list are null, none of them should be
     * added (i.e. either all of the stops are added, or none are).
     *
     * @param stops The stops to add to the network.
     */
    public void addStops(List<Stop> stops) {
        if (stops.contains(null)) {
            return;
        }

        this.stops.addAll(stops);
    }

    /**
     * Adds the given vehicle to the network.
     * If the given vehicle is null, it should not be added to the network.
     *
     * @param vehicle The vehicle to add to the network.
     */
    public void addVehicle(PublicTransport vehicle) {
        if (vehicle == null) {
            return;
        }

        vehicles.add(vehicle);
    }

    /**
     * Gets all the routes in this network.
     * Modifying the returned list should not result in changes to the internal
     * state of the class.
     *
     * @return All the routes in the network.
     */
    public List<Route> getRoutes() {
        return routes;
    }

    /**
     * Gets all of the stops in this network.
     * Modifying the returned list should not result in changes to the internal
     * state of the class.
     *
     * @return All the stops in the network.
     */
    public List<Stop> getStops() {
        return stops;
    }

    /**
     * Gets all the vehicles in this transportation network.
     * Modifying the returned list should not result in changes to the internal
     * state of the class.
     *
     * @return All the vehicles in the transportation network.
     */
    public List<PublicTransport> getVehicles() {
        return vehicles;
    }

    /**
     * Saves this network to the file indicated by the given filename.
     * The file should be written with the same format as described in the
     * Network(String) constructor.
     *
     * The stops should be written to the file in the same order in which they
     * were added to the network. This also applies to the routes and the
     * vehicles.
     *
     * If the given filename is null, the method should do nothing.
     *
     * @param filename The name of the file to save the network to.
     * @throws IOException If there are any IO errors whilst writing to the
     *                      file.
     */
    public void save(String filename) throws IOException {
        try {
            BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
            writer.write(this.toString());
            writer.close();
        } catch (IOException e) {
            System.out.println("Error opening file");
        }
    }
}
