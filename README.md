STM32F030F4P6 based battery charger for LiPol 2S (Can charge battery from USB or 3S battery)

The charger schematics is in Schematics directory.

This charger uses STM32F030F4P6 MCU to control buck-boost converter to
recharge Li-Pol batteries from USB or 12V (3S battery, car battery, etc)
power source. It may control MP1584EN based step-down converter to generate
5V for USB-powered stuff (pin 2 of MP1584EN must be connected to the "DC-EN"
output.

It doesn't support fast charging and stuff like that, as current limit is
about 200 mA because quite simple schematics.

Note, that it has balance check but doesn't do real battery balancing (just
to prevent overcharge). You will need to do periodic rebalancing on a real
charger.

These sources are the Eclipse's project (with GNU ARM plugin)
