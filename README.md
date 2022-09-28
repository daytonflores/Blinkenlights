# PES-Assignment-3
 
## Peer Code Review

### Reviewer - Mark Sherman

- ESE Coding Guidelines
	- file description / author missing
	- code block brackets not started on new line
- LED/TSI macros are a little hard to read, could be implemented in separate functions / source files for a more clear definition
- debug messages make readability a bit difficult
- GET_LED_COLOR:
	- kind of confusing flow, updates LED color and also turns it on if the color changed, but a separate LED_ON macro is declared
- init_onboard_leds
	- Line 275: turns LED on (again) instead of off for 100ms
	- Line 288: turns LED on (again) instead of off for 100ms
- init_onboard_touch_sensor: 
	- tsi general config registers bit manipulations are a little hard to read 
- blink_sequence:
	- could use #defines for timer counts (5, 10, etc), or write a function that could be reused for different delay times
	- first sequence won't update LED color if tsi reads a value (before while(1) loop is entered)
	- if color changes during off delay, GET_LED_COLOR will turn the LED on again to the new color
