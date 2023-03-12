import 'dart:collection';

import 'dart:async';
import 'dart:collection';
import 'dart:math';
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';

void main() => runApp(MyApp());
HashMap<int, HashMap<String,String>> inspector = new HashMap();
BuildContext currentContext;
class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // Try running your application with "flutter run". You'll see the
        // application has a blue toolbar. Then, without quitting the app, try
        // changing the primarySwatch below to Colors.green and then invoke
        // "hot reload" (press "r" in the console where you ran "flutter run",
        // or simply save your changes to "hot reload" in a Flutter IDE).
        // Notice that the counter didn't reset back to zero; the application
        // is not restarted.
        primarySwatch: Colors.red,
      ),
      home: MyHomePage(
          title: 'I N S P E C T A',
         ),

    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key key, this.title}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _counter = 0;
  String _info = "You haven't pressed the button";
  void _incrementCounter() {
    setState(() {
      // This call to setState tells the Flutter framework that something has
      // changed in this State, which causes it to rerun the build method below
      // so that the display can reflect the updated values. If we changed
      // _counter without calling setState(), then the build method would not be
      // called again, and so nothing would appear to happen.
      _counter++;
    });
  }

  void _displayInfo() {
    setState(() {
      _info = "ben pressed the button";
    });
  }

  void _reset() {
    setState(() {
      _info = "Button was Reset";
    });
  }

  @override
  Widget build(BuildContext context) {
    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.

    Widget selectSection = Container(
      padding: const EdgeInsets.all(32),
      child: Column(
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              Text(
                'What are you here for?',
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  fontSize: 25,

                ),
              ),
            ],
          ),
          new Container(
            height: 20,
          ),
          Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                new RaisedButton(
                  padding: const EdgeInsets.all(20.0),
                  textColor: Colors.white,
                  color: Colors.red,
                  onPressed: () {
                    /*
                    _navigateAndDisplayInspector();
                     */
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => SecondRoute()),
                    );
                  },
                  child: Text("Inspector Account",
                    style: TextStyle(
                    fontWeight: FontWeight.bold,
                    fontSize: 15,
                    )
                  ),
                ),

                new RaisedButton(
                  padding: const EdgeInsets.all(20.0),
                  textColor: Colors.white,
                  color: Colors.red,
                    onPressed: () {
                    Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => ThirdRoute()),
                    );
                    },
                  child: new Text("Inspector Search",
                      style: TextStyle(
                        fontWeight: FontWeight.bold,
                        fontSize: 15,
                      )
                  ),
                ),
              ]
          ),
          new Container(
            height: 20,
          ),
//          Row(
//            children: <Widget>[
//              new RaisedButton(
//                padding:const EdgeInsets.all(8.0),
//                textColor: Colors.white,
//                color: Colors.red,
//                onPressed: () {
//                  Navigator.push(
//                    context,
//                    MaterialPageRoute(builder: (context) => ThirdRoute()),
//                  );
//                },
//                child: new Text("MAP"),
//              ),
//            ],
//          ),
      Container(
        width: 100,
        height: 100,
        child: Image.asset(
          'images/add01.jpg',
          //'images/Unknown.jpg',
          fit: BoxFit.fitHeight,
        ),
      ),
//          Image.asset(
//            'images/Unknown.jpg',
//            width: 100,
//            height: 156,
//            fit: BoxFit.fitWidth,
//          ),
//          new Container(
//            height: 120.0,
//            width: 120.0,
//            decoration: new BoxDecoration(
//              image: DecorationImage(
//                image: new AssetImage(
//                    'images/maxresdefault.jpg'),
//                fit: BoxFit.fill,
//              ),
//            ),
//          ),
        ],
      ),
    );
    return Scaffold(
        appBar: AppBar(
          // Here we take the value from the MyHomePage object that was created by
          // the App.build method, and use it to set our appbar title.
          title: Text(widget.title),
          centerTitle: true,
          textTheme: TextTheme(
          title: TextStyle(
          color: Colors.white,
          fontSize: 30.0,)
          )
        ),
        body: ListView(
            children: [
              Image.asset(
                'images/Unknown.jpg',
                width: 100,
                height: 156,
                fit: BoxFit.fitWidth,
              ),
              selectSection,

            ]

        )
      // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
class SecondRoute extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Inspector Information',
      home: InspectorForm(),
    );
  }
/*{
    return Scaffold(
      appBar: AppBar(
        title: Text("Second Route"),
      ),
      body: Center(
        child: RaisedButton(
          onPressed: () {
            Navigator.pop(context);
            // Navigate back to first route when tapped.
          },
          child: Text('Go back!'),
        ),
      ),
    );
  }*/
}

class InspectorForm extends StatefulWidget {
  @override
  _InspectorFormState createState() => _InspectorFormState();
}

class _InspectorFormState extends State<InspectorForm> {
  final nameController = TextEditingController();
  final emailController = TextEditingController();
  final postcodeController = TextEditingController();

  @override
  void dispose() {
    nameController.dispose();
    emailController.dispose();
    postcodeController.dispose();
    super.dispose();
  }

  void empty() {
    nameController.text = '';
    emailController.text = '';
    postcodeController.text = '';
  }

  void submit() {
    int postCode;
    postCode = int.parse(postcodeController.text);
    HashMap<String, String> information = new HashMap();
    String name;
    String email;
    name = nameController.text;
    email = emailController.text;
    if (inspector.containsKey(postCode)) {
      (inspector[postCode])[name] = email;
    }
    else {
      information[name] = email;
      inspector[postCode] = information;
    }

    print("$inspector");


  }
  @override
  Widget build(BuildContext context) {
    //currentContext = context;
    return Scaffold(
      appBar: AppBar(
        title: Text('Inspector Account Creation'),
        centerTitle: true,
      ),
      body: ListView(
        children: <Widget>[
          new Container(
            height: 40,
          ),
          new Container(
            height: 20,
            alignment: Alignment(0.0, 0.0),
            child: Text("Want to become an Inspector? Enter Your details below to begin")
          ),
          new Container(
            height: 40,
          ),
          Text(
            'Your Name:',
            style: TextStyle(
              color: Colors.grey[500],
            ),
          ),
          TextField(
            controller: nameController,
          ),
          Text(
            'Your Email',
            style: TextStyle(
              color: Colors.grey[500],
            ),
          ),
          TextField(
            controller: emailController,
          ),
          Text(
            'Your Postcode',
            style: TextStyle(
              color: Colors.grey[500],
            ),
          ),
          TextField(
            controller: postcodeController,
          ),
          
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              new RaisedButton(
                padding: const EdgeInsets.all(4.0),
                textColor: Colors.white,
                color: Colors.blue,
                onPressed: () {
                  submit();
                },
                child: new Text("Submit"),
              ),
              new RaisedButton(
                padding: const EdgeInsets.all(8.0),
                textColor: Colors.white,
                color: Colors.blue,
                onPressed: empty,
                child: new Text("Clear"),
              ),
            ],
          ),
          new Container(
              height: 40,
          ),
          new Container(
            height: 20,
              alignment: Alignment(0.0, 0.0),
              child: Text("Already have an Account? Click Below to Log in")
          ),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              new RaisedButton(
                padding: const EdgeInsets.all(4.0),
                textColor: Colors.white,
                color: Colors.blue,
                onPressed: submit,
                child: new Text("Log In"),
              ),
            ],
          ),
        ],
      ),
    );
    // TODO: implement build
    return null;
  }
}

class ThirdRoute extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Mapp',
      home: MyApp2(),
    );
  }
/*{
    return Scaffold(
      appBar: AppBar(
        title: Text("Second Route"),
      ),
      body: Center(
        child: RaisedButton(
          onPressed: () {
            Navigator.pop(context);
            // Navigate back to first route when tapped.
          },
          child: Text('Go back!'),
        ),
      ),
    );
  }*/
}

/**
* Implement GoogleMaps
 */

class MyApp2 extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp2> {
  Completer<GoogleMapController> _controller = Completer();

  //static const LatLng _center = const LatLng(-27.40125, 153.021072);
  static const LatLng _center = const LatLng(-27.41125, 153.021072);

  List<LatLng> posList = new List<LatLng>();

  final Set<Marker> _markers = {};

  LatLng _lastMapPosition = _center;

  MapType _currentMapType = MapType.normal;

  double offset = 0.01;
  double latitude = -27.41125;
  double longitude = 153.021072;

  void _onMapTypeButtonPressed() {
    setState(() {
      _currentMapType = _currentMapType == MapType.normal
          ? MapType.satellite
          : MapType.normal;
    });
  }

  void _onAddMarkerButtonPressed() {
    posList.add(_generate_lat_lng());
    print(posList);
    setState(() {
      _markers.clear();
      for (LatLng pos in posList) {
        _markers.add(Marker(
          markerId: MarkerId(pos.toString()),
          position: pos,
          infoWindow: InfoWindow(
            title: 'Really cool place',
            snippet: '5 Star Rating',
          ),
          icon: BitmapDescriptor.defaultMarker,
        ));
      }
    });
  }

  LatLng _generate_lat_lng() {
    var rng = new Random();
    int sign = rng.nextInt(4);
    double x = rng.nextDouble()/100;
    double y = rng.nextDouble()/100;

    double latitude = this.latitude;
    double longitude = this.longitude;

    if (sign > 3) {
      latitude += x;
      longitude += y;
    } else if (sign > 2) {
      latitude -= x;
      longitude -= y;
    } else if (sign > 1) {
      latitude -= x;
      longitude += y;
    } else{
      latitude += x;
      longitude -= y;
    }
//    latitude += offset;
//    longitude += offset;

    return LatLng(latitude, longitude);

  }

  void _onCameraMove(CameraPosition position) {
    _lastMapPosition = position.target;
  }

  void _onMapCreated(GoogleMapController controller) {
    _controller.complete(controller);
  }

  @override
  Widget build(BuildContext context) {

//    posList.add(LatLng(-27.41125, 153.021072));
//    posList.add(LatLng(-27.3859, 153.0295));
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text('Maps Sample App'),
          backgroundColor: Colors.green[700],
        ),
        body: Stack(
          children: <Widget>[
            GoogleMap(
              onMapCreated: _onMapCreated,
              initialCameraPosition: CameraPosition(
                target: _center,
                zoom: 11.0,
              ),
              mapType: _currentMapType,
              markers: _markers,
              onCameraMove: _onCameraMove,
            ),
            Padding(
              padding: const EdgeInsets.all(16.0),
              child: Align(
                alignment: Alignment.topRight,
                child: Column(
                  children: <Widget> [
                    FloatingActionButton(
                      heroTag: "btn1",
                      onPressed: _onMapTypeButtonPressed,
                      materialTapTargetSize: MaterialTapTargetSize.padded,
                      backgroundColor: Colors.green,
                      child: const Icon(Icons.map, size: 36.0),
                    ),
                    SizedBox(height: 16.0),
                    FloatingActionButton(
                      heroTag: "btn2",
                      onPressed: _onAddMarkerButtonPressed,
                      materialTapTargetSize: MaterialTapTargetSize.padded,
                      backgroundColor: Colors.green,
                      child: const Icon(Icons.add_location, size: 36.0),
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}