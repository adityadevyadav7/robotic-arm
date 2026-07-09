# Notes

Servo jitter was caused by loose wires.

Always use external 5V supply.

Ground must be common.

Current and Target should always be different variables.

Motion Engine should only update Current.

Command functions only update Target.

Never use servo.write() directly inside command functions.

delay() works for now.

Later replace with millis().
