# tradingenginecpp

## Purpose
Leveraging C++ knowledge as well as technically analysis to create an automated trading enigne.  Monitoring indictaors through orderbook data and calculating moving averages in real-time, this system will trade based on a user-defined set of parameters entered in the engine startup script.

## Theory
3 concerns
1. Entry Trigger
2. Exit Trigger
3. Thesis

Thesis: When moving average dips below a user-defined S/L level and the gradient of at MA flips before the level is reclaimed on the market basis then trigger entry.

## Usage
1. Compile using g++ or use pre-compiled on same OS
2. Fill out run_engine.sh with desired parameters
3. Run `./run_engine.sh`

