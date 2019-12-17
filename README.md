# Ananke
Interactive Dance Performance in OpenFrameworks (Made In Collaboration with Mari Calderon and Erica Wu)




Ananke, a reference to the greek personification of inevitability, compulsion, and necessity, deals with the double-edged nature of technology, in the
midst of a society where the lines between technology, surveillance, and data privacy get increasingly blurred. The piece deals with the interactions
between a dancer and a mysterious dodecahedron hanging from the stage ceiling, triggering sounds and visuals throughout the performance. The performance
starts serene and playful as the dancer gets used to the seemingly innocent dodecahedron (which represents technology and social media). However,
as she focus on it and bends its powers to her will, a hidden figure emerges, suggesting that what initially seem harmless might not actually be so.

This interactive performance uses live tracking of an infrared light using a PS3Eye Camera. Using a ring of bright infrared
LEDs inside the semi-opaque dodecahedron allowed this to be possible. Apart from the infrared lights inside the dodecahedron, 
we also decided to use a neopixel ring. We added this to make the dodecahedron appear like something alive, changing colors twice during 
the performance. We made this possible by enabling the Neopixel ring to be remotely via a Bluetooth LE and the Adafruit application. 

The visuals were all programmed in C++ using OpenFrameworks with some prototyping done  
in Processing. We decided to use particle animations since we believed this would be the most flexible
throughout the performance, allowing us to vary their movement and color depending on the stage
of the story. 
