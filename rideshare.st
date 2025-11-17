"=== Ride base class ==="
Object subclass: #Ride
    instanceVariableNames: 'rideID pickupLocation dropoffLocation distance'
    classVariableNames: ''
    poolDictionaries: ''
    category: 'RideSharing'.

Ride >> initialize
    super initialize.
    rideID := ''.
    pickupLocation := ''.
    dropoffLocation := ''.
    distance := 0.0.

Ride >> rideID: anID
    rideID := anID.

Ride >> pickupLocation: loc
    pickupLocation := loc.

Ride >> dropoffLocation: loc
    dropoffLocation := loc.

Ride >> distance: km
    distance := km.

Ride >> fare
    "Default fare formula: overridden in subclasses"
    ^ distance * 1.0

Ride >> rideDetails
    ^ String streamContents: [:s |
        s nextPutAll: 'Ride ID: ', rideID; cr;
          nextPutAll: 'Pickup: ', pickupLocation; cr;
          nextPutAll: 'Dropoff: ', dropoffLocation; cr;
          nextPutAll: 'Distance(km): ', distance printString; cr;
          nextPutAll: 'Fare: $', (self fare) printString ].

"=== StandardRide subclass ==="
Ride subclass: #StandardRide
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: ''
    category: 'RideSharing'.

StandardRide >> fare
    | baseRate perKm |
    baseRate := 2.0. "flat start"
    perKm := 1.5.
    ^ baseRate + (perKm * distance).

"=== PremiumRide subclass ==="
Ride subclass: #PremiumRide
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: ''
    category: 'RideSharing'.

PremiumRide >> fare
    | baseRate perKm premiumMultiplier |
    baseRate := 4.0.
    perKm := 2.5.
    premiumMultiplier := 1.5.
    ^ (baseRate + (perKm * distance)) * premiumMultiplier.

"=== Driver class ==="
Object subclass: #Driver
    instanceVariableNames: 'driverID name rating assignedRides'
    classVariableNames: ''
    poolDictionaries: ''
    category: 'RideSharing'.

Driver >> initialize
    super initialize.
    assignedRides := OrderedCollection new.
    rating := 5.0.
    driverID := ''.
    name := ''.

Driver >> driverID: anID name: aName rating: r
    driverID := anID.
    name := aName.
    rating := r.

Driver >> addRide: aRide
    "Encapsulated access to assignedRides"
    assignedRides add: aRide.

Driver >> getDriverInfo
    ^ String streamContents: [:s |
        s nextPutAll: 'Driver: ', name; cr;
        nextPutAll: 'ID: ', driverID; cr;
        nextPutAll: 'Rating: ', rating printString; cr;
        nextPutAll: 'Assigned rides: '; cr;
        assignedRides do: [:r | s nextPutAll: (r rideDetails); cr ] ].

"=== Rider class ==="
Object subclass: #Rider
    instanceVariableNames: 'riderID name requestedRides'
    classVariableNames: ''
    poolDictionaries: ''
    category: 'RideSharing'.

Rider >> initialize
    super initialize.
    requestedRides := OrderedCollection new.
    riderID := ''.
    name := ''.

Rider >> riderID: anID name: aName
    riderID := anID.
    name := aName.

Rider >> requestRide: aRide
    requestedRides add: aRide.

Rider >> viewRides
    ^ String streamContents: [:s |
        s nextPutAll: 'Rider: ', name; cr;
        s nextPutAll: 'Requested rides: '; cr;
        requestedRides do: [:r | s nextPutAll: (r rideDetails); cr ] ].

"=== Demo (create rides, driver, rider, show polymorphism) ==="
| r1 r2 r3 driver rider ridesCollection |
r1 := StandardRide new.
r1 rideID: 'S-001'; pickupLocation: 'Market St'; dropoffLocation: 'Union Sq'; distance: 5.

r2 := PremiumRide new.
r2 rideID: 'P-001'; pickupLocation: 'Embarcadero'; dropoffLocation: 'Ferry Building'; distance: 3.

r3 := StandardRide new.
r3 rideID: 'S-002'; pickupLocation: 'Mission'; dropoffLocation: 'Castro'; distance: 4.2.

driver := Driver new.
driver driverID: 'D100' name: 'Lee' rating: 4.9.
driver addRide: r1.
driver addRide: r2.

rider := Rider new.
rider riderID: 'R200' name: 'Asha'.
rider requestRide: r1.
rider requestRide: r3.

"Polymorphic collection"
ridesCollection := { r1. r2. r3 }.

Transcript show: '--- Polymorphic fares and details ---'; cr.
ridesCollection do: [:ride |
    Transcript show: ride rideDetails; cr ].

Transcript show: driver getDriverInfo; cr.
Transcript show: rider viewRides; cr.
