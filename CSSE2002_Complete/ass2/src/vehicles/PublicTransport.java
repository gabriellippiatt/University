package vehicles;

import exceptions.EmptyRouteException;
import exceptions.IncompatibleTypeException;
import exceptions.OverCapacityException;
import exceptions.TransportFormatException;
import passengers.Passenger;
import routes.BusRoute;
import routes.Route;
import stops.Stop;
import sun.invoke.empty.Empty;
import utilities.Writeable;

import java.util.ArrayList;
import java.util.List;

/**
 * A base public transport vehicle in the transportation network.
 */
public abstract class PublicTransport implements Writeable {
    // the passengers currently on board the vehicle
    private List<Passenger> passengers;

    // the place the vehicle is currently stopped
    private Stop currentLocation;

    // the maximum passengers allowed on board the vehicle
    private int capacity;

    // the vehicle's identifier
    private int id;

    // the route the vehicle follows
    private Route route;

    /**
     * Creates a new public transport vehicle with the given id, capacity, and
     * route.
     *
     * <p>The vehicle should initially have no passengers on board, and should be placed
     * at the beginning of the given route (given by {@link Route#getStartStop()}).
     * If the route is empty, the current location should be stored as null.
     *
     * <p> If the given capacity is negative, 0 should be stored as the capacity
     * instead (meaning no passengers will be allowed on board this vehicle).
     *
     * @param id The identifying number of the vehicle.
     * @param capacity The maximum number of passengers allowed on board.
     * @param route The route the vehicle follows. Note that the given route should
     *              never be null (@require route != null), and thus will not be
     *              tested with a null value.
     */
    public PublicTransport(int id, int capacity, Route route) {
        this.passengers = new ArrayList<>();
        this.capacity = capacity < 0 ? 0 : capacity;
        this.id = id;
        this.route = route;
        try {
            this.currentLocation = route.getStartStop();
        } catch (EmptyRouteException e) {
            this.currentLocation = null;
        }
    }

    /**
     * Returns the route this vehicle is on.
     *
     * @return The route this vehicle is on.
     */
    public Route getRoute() {
        return route;
    }

    /**
     * Returns the id of this vehicle.
     *
     * @return The id of this vehicle.
     */
    public int getId() {
        return id;
    }

    /**
     * Returns the current location of this vehicle.
     *
     * @return The stop this vehicle is currently located at, or null if it is not
     *         currently located at a stop.
     */
    public Stop getCurrentStop() {
        return currentLocation;
    }

    /**
     * Returns the number of passengers currently on board this vehicle.
     *
     * @return The number of passengers in the vehicle.
     */
    public int passengerCount() {
        return passengers.size();
    }

    /**
     * Returns the maximum number of passengers allowed on this vehicle.
     *
     * @return The maximum capacity of the vehicle.
     */
    public int getCapacity() {
        return capacity;
    }

    /**
     * Returns the type of this vehicle, as determined by the type of the route it
     * is on (i.e. The type returned by {@link Route#getType()}).
     *
     * @return The type of this vehicle.
     */
    public String getType() {
        return route.getType();
    }

    /**
     * Returns the passengers currently on-board this vehicle.
     *
     * <p>No specific order is required for the passenger objects in the returned
     * list.
     *
     * <p>Modifying the returned list should not result in changes to the internal
     * state of the class.
     *
     * @return The passengers currently on the public transport vehicle.
     */
    public List<Passenger> getPassengers() {
        return new ArrayList<>(passengers);
    }

    /**
     * Adds the given passenger to this vehicle.
     *
     * <p>If the passenger is null, the method should return without adding it
     * to the vehicle.
     *
     * <p>If the vehicle is already at (or over) capacity, an exception should
     * be thrown and the passenger should not be added to the vehicle.
     *
     * @param passenger The passenger boarding the vehicle.
     * @throws OverCapacityException If the vehicle is already at (or over) capacity.
     */
    public void addPassenger(Passenger passenger) throws OverCapacityException {
        if (passenger == null) {
            return;
        }

        if (passengers.size() >= capacity) {
            throw new OverCapacityException();
        }
        passengers.add(passenger);
    }

    /**
     * Removes the given passenger from the vehicle.
     *
     * <p>If the passenger is null, or is not on board the vehicle, the method should
     * return false, and should not have any effect on the passengers currently
     * on the vehicle.
     *
     * @param passenger The passenger disembarking the vehicle.
     * @return True if the passenger was successfully removed, false otherwise (including
     *         the case where the given passenger was not on board the vehicle to
     *         begin with).
     */
    public boolean removePassenger(Passenger passenger) {
        return passengers.remove(passenger);
    }

    /**
     * Empties the vehicle of all its current passengers, and returns all the passengers
     * who were removed.
     *
     * <p>No specific order is required for the passenger objects in the returned
     * list.
     *
     * <p>If there are no passengers currently on the vehicle, the method just
     * returns an empty list.
     *
     * <p>Modifying the returned list should not result in changes to the internal
     * state of the class.
     *
     * @return The passengers who used to be on the vehicle.
     */
    public List<Passenger> unload() {
        List<Passenger> leaving = passengers;
        passengers = new ArrayList<>();
        return leaving;
    }

    /**
     * Updates the current location of the vehicle to be the given stop.
     *
     * <p>If the given stop is null, or is not on this public transport's route
     * the current location should remain unchanged.
     *
     * @param stop The stop the vehicle has travelled to.
     */
    public void travelTo(Stop stop) {
        if (!route.getStopsOnRoute().contains(stop)) {
            return;
        }

        currentLocation = stop == null ? currentLocation : stop;
    }

    /**
     * Creates a string representation of a public transport vehicle in the format:
     *
     * <p>'{type} number {id} ({capacity}) on route {route}'
     *
     * <p>without the surrounding quotes, and where {type} is replaced by the type of
     * the vehicle, {id} is replaced by the id of the vehicle, {capacity} is replaced
     * by the maximum capacity of the vehicle, and {route} is replaced by the route
     * number of the route the vehicle is on. For example:
     *
     * <p>bus number 1 (30) on route 1
     *
     * @return A string representation of the vehicle.
     */
    @Override
    public String toString() {
        return getType() + " number " + id + " (" + capacity + ") on route " + route.getRouteNumber();
    }

    /**
     * Creates a new public transport object based on the given string
     * representation.
     * The format of the given string should match that returned by the encode()
     * method, with one modification:
     *
     * '{type},{id},{capacity},{route},{extra}'
     *
     * where all parts of the string are as defined in encode(), except for
     * {extra}, which should be replaced with a different part depending on the
     * type of vehicle being decoded (as given by {type}).
     *
     * If {type} is bus, then {extra} should be the registration number of the
     * bus.
     * If {type} is train, then {extra} should be the carriage count of the
     * train.
     * If {type} is ferry, then {extra} should be the type of the ferry.
     *
     * The decoded vehicle should be added to the Route indicated by {route}
     * using the Route.addTransport(PublicTransport) method. If there are
     * multiple routes in the existingRoutes list which have the same number,
     * then the first route in the list with a matching number should be used.
     *
     * Whilst parsing, if spaces (i.e. ' ') are encountered before or after
     * integers, (e.g. {id}), the spaces should simply be trimmed (for example,
     * using something like String.trim()). If spaces are encountered before or
     * after strings (e.g. {type}), the spaces should be considered part of the
     * string and not handled differently from any other character.
     *
     * @param transportString The string to decode.
     * @param existingRoutes The routes which currently exist in the transport
     *                       network.
     * @return The decoded public transport object (a Bus, Train, or Ferry,
     * depending on the type given in the string).
     * @throws TransportFormatException If the given string or existingRoutes
     * list is null, or the string is otherwise incorrectly formatted (according
     * to the encode() representation). This includes, but is not limited to:
     *      1 - A transport type that is not one of "bus", "train", or "ferry".
     *      2 - One of the id, capacity, or route number is not an integer
     *          value.
     *      3 - The route number given in the string does not match one of the
     *          given existingRoutes.
     *      4 - The type of the route referenced in the given string does not
     *          match the type given in the transportString (e.g. a Bus
     *          referencing a TrainRoute).
     *      5 - A vehicle of type train whose {extra} part (i.e. carriage count)
     *          is not an integer value.
     *      6 - An error (i.e. EmptyRouteException or IncompatibleTypeException
     *          is encountered whilst adding the vehicle to its route
     *      7 - Any extra delimiters (,) being encountered whilst parsing.
     *      8 - Any of the parts of the string being missing.
     */
    public static PublicTransport decode(String transportString,
            List<Route> existingRoutes) throws TransportFormatException {
        int id;
        int capacity;
        int routeNumber;
        int carriage = 0;
        Route current = null;
        PublicTransport vehicle;
        String type;

        if (transportString == null) {
            throw new TransportFormatException();
        }

        if (existingRoutes == null) {
            throw new TransportFormatException();
        }

        String[] stringComponents = transportString.split(",");

        for (String component : stringComponents) {
            if (component == null) {
                throw new TransportFormatException();
            }
        }

        if (stringComponents.length > 5 || stringComponents.length < 4) {
            throw new TransportFormatException();
        }

        type = stringComponents[0];

        try {
            id = Integer.parseInt(stringComponents[1].trim());
        } catch(NumberFormatException e) {
            throw new TransportFormatException();
        }

        try {
            capacity = Integer.parseInt(stringComponents[2].trim());
        } catch(NumberFormatException e) {
            throw new TransportFormatException();
        }

        try {
            routeNumber = Integer.parseInt(stringComponents[3].trim());
        } catch(NumberFormatException e) {
            throw new TransportFormatException();
        }

        ArrayList<Integer> existingRouteNumbers = new ArrayList<>();

        for (Route route : existingRoutes) {
            int number = route.getRouteNumber();
            existingRouteNumbers.add(number);
        }

        if (!existingRouteNumbers.contains(routeNumber)) {
            throw new TransportFormatException();
        }

        String extra = stringComponents[4];

        if (type == "train") {
            carriage = Integer.parseInt(extra);
        }

        for (Route route : existingRoutes) {
            if (route.getRouteNumber() == routeNumber) {
                current = route;
            } else {
                throw new TransportFormatException();
            }
        }

        if (!current.getType().equals(type)) {
            throw new TransportFormatException();
        }

        try {
            if (type.equals("bus")) {
                vehicle = new Bus(id,capacity,current,extra);

            } else if (type.equals("ferry")) {
                vehicle = new Ferry(id,capacity,current,extra);

            } else if (type.equals("train")) {
                vehicle = new Train(id,capacity,current,carriage);

            } else {
                throw new TransportFormatException();
            }

            current.addTransport(vehicle);
            return vehicle;

        } catch (EmptyRouteException | IncompatibleTypeException e) {
            throw new TransportFormatException();
        }
    }

    /**
     * Encodes this vehicle as a string in the format:
     *
     * <p>'{type},{id},{capacity},{route}'
     *
     * <p>without the surrounding quotes, and where {type} is replaced by the
     * type of the vehicle, {id} is replaced by the id of the vehicle,
     * {capacity} is replaced by the maximum capacity of the vehicle, and
     * {route} is replaced by the route number of the route the vehicle is on.
     * For example:
     *
     * <p>bus,1,30,1
     *
     * @return This vehicle encoded as a string.
     */
    @Override
    public String encode() {
        return getType() + "," + getId() + "," + getCapacity() + "," +
                getRoute();
    }
}
