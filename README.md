# units #

A toy library to handle unit conversions.

## Philosophy ##

Confusion about units has bitten most programmers at some point in their career. It even caused NASA to miss Mars one time. Getting units wrong should be hard (if not impossible). This is my humble attempt to implement a library generalizing the work done in STL with the chrono library.

## The Code ##

Is ugly, opaque, and possibly wrong. I won't kid myself. But my hope is that it will (eventually) be easy to use and will be hard/impossible to use incorrectly (because that leads to rockets whiffing planets).

    using namespace units;
    using inches = distance::inches<float>;
    using millimeters = distance::millimeters<float>;
    auto length_in_inches = inches(5);
    millimeters length_in_mm = length_in_inches; // implicitly converted
    auto length_in_mm2 = units_cast<millimeters>(length_in_inches); // explicitly converted
