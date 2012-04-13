=====================================================================

        ALGORITHMIC PAIRS TRADING - README
        Github repository: https://github.com/TejasM/Algotrading

---------------------------------------------------------------------

This is the documentation for the Pairs Trading Algorithm Implementation.
See also DesignDocument.pdf

Group 04
Tejas Mehta         997486581
Neil Newman         997382443
Stefan Hadjis       997701170

Submission for ECE358

=====================================================================

The following key files are included:
-------------------------------------

The release includes an executable, test files, as well as all source code.
The project can be built using Visual Studio 2010 (perhaps earlier versions).
(All locations with respect to the Algotrading/ directory)

AlgoTrading.vcxproj
- The Visual Studio 2010 Project file

./Release/AlgoTrading.exe
- The executable GUI
- See below for using the GUI

./TestSocketClient
- The source code for the project

./Examples
- Contains example input input files for PairsTrading and EMA, MACD.


=====================================================================

Steps to use the GUI
--------------------

1. Run: ./Release/AlgoTrading.exe
2. Ensure that you are logged into TWS
3. Click Connect and accept incoming connection attempt
4. Click Input and specify an action code and input file Path


IMPORTANT NOTE: This path must be an absolute directory, e.g.:


        C:\ECE358\Algotrading\Examples\pair.txt


5. See DesignDocument.pdf for information on action codes and input,
   as well as the Examples folder for example include files.
        
Outputs that require file output will be in the form of output files 
written to the root directory. Other data such as account requests, 
errors, and small unimportant data will be displayed their respective 
list boxes in the GUI.

Another thing to note is at times the client returns the following 
error message: Error Code: 200| Error Msg: No security definition has
been found for the request. On researching the problem we have come 
to the conclusion that this is not our fault and is due to
disconnection of the TWS from the server while recieve data.

Bar Sizes in multiples of 5 are the only bar sizes supported. Even
then the real time data will be printed every 5 secs but the EMA
outpu files will only be updated every bar size seconds (though) it
will still say 5s data.

=====================================================================

Summary of Input Files
----------------------

canemacrossover.txt
- Cancel the EMA crossover. 
- Contains the id of the one that we want to stop.

canemacrossverstopwin.txt
- Cancel the EMA crossover with stop win/loss. 
- Contains the id of the one that we want to stop.

canpair.txt
- Cancel the pair tradings
- Contains the id of the first id fo the pairs

emacstopwin.txt
- Start the ema crossover with stop win/loss strategy
- Inputs are as follows: id, stock, order size, d1, d2, fast, slow

emac.txt
- Start with ema crossover strategy
- Inputs are as follows: id, stock, order size, fast, slow

pair.txt
- Start the pairs trading strategy
- Inputs are as follows: id1, stock1, id2, stock2, id3, stock3, id4, stock4, id5, stock5, id6, stock6

order.txt
- Place an order
- Inputs are as follows: order id, stock, order action, order size, order type, lmt price

=====================================================================

List of Key Source Files - please read alongside comments in the code
---------------------------------------------------------------------

The following source files in ./TestSocketClient contain the implementation
of our algorithm. Below is a short description of each, but see the
comments in each file as a main source of documentation.

1. PairsTrading.h and PairsTrading.cpp
- Implementation of our algorithmic Pairs Trading algorithm
- See Design Document: This implements the a four state Finite State Machine

2. Stock.h, Stock.cpp
- Defines the stock object. In addition to keeping information such as the 
current EMA and MACD objects associated with each stock, this class writes 
the indicator data to files and places orders.

3. EMA.h, EMA.cpp
- Contains implementation of both MACD and EMA classes which are instantiated 
by the stock class.

4. EMACrossover.h, EMACrossover.cpp
- Implements the EMACrossover strategy both the stop win/loss and the standard 
crossover strategy.

5. client2Dlg.h, client2Dlg.cpp 
- Changes made to implement the GUI and Communication Layer
- The main function of the Communication Layer is the parseFunction which deals with 
each action code and performs the necessary action.
