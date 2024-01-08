**Stun Server**

Stun Server is the implementation of the IETF's Session Traversal Utilities for NAT (STUN) (RFC 5389) .

https://datatracker.ietf.org/doc/html/rfc5389

**Requirements**

XmlParser - v1.0

https://github.com/praveen-kd-23/XmlParser

https://github.com/praveen-kd-23/XmlParser/releases

**Build Instructions**

```

Download the latest xmlparser release build in the main folder .

make

```

**Running the application**

Configure the host , port in conf/stun.xml

```
./stunserver
```

**Testing**

1 . Open https://webrtc.github.io/samples/src/content/peerconnection/trickle-ice/

2 . Add the server url in STUN or TURN URI with the below format
    stun:public_address_of_your_machine:port 
    eg : stun:183.24.53.45:3478
    Click Add Server

3 . Click Gather Candidates .
    srflx candidates will be generated . 
