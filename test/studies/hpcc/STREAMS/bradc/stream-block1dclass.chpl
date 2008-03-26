use Time, Types, Random;
use BradsBlock1D;

use HPCCProblemSize;


param numVectors = 3;
type elemType = real(64),
     indexType = int(64);

config const m = computeProblemSize(elemType, numVectors),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 0.0;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.clockMS else 314159265;

config const printParams = true,
             printArrays = false,
             printStats = true;


def main() {
  printConfiguration();

  const ProblemSpace = new Block1DDom([1..m]);

  var A = ProblemSpace.newArray(elemType), 
      B = ProblemSpace.newArray(elemType),
      C = ProblemSpace.newArray(elemType);

  initVectors(B, C);

  var execTime: [1..numTrials] real;

  for trial in 1..numTrials {
    const startTime = getCurrentTime();
    // TODO: Want    A = B + alpha * C;

    /* TODO OR EVEN: -- but this fails due to "detupled indices drop
       references to var iterators bug
       
    forall (a, b, c) in (A, B, C) {
      a = b + alpha * c;
    }
    */
    forall (i, b, c) in (ProblemSpace, B, C) {
      A(i) = b + alpha * c;
    }
    execTime(trial) = getCurrentTime() - startTime;
  }  

  const validAnswer = verifyResults(A, B, C);
  printResults(validAnswer, execTime);
}


def printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}


def initVectors(B, C) {
  var randlist = new RandomStream(seed);

  // TODO: should write a fillRandom() implementation that does this
  coforall loc in LocaleSpace {
    // TODO: Need to clean this up to use more normal method names
    randlist.skipToNth(B.locArr(loc).locDom.low);
    randlist.fillRandom(B.locArr(loc).myElems);
    randlist.skipToNth(B.numElements + C.locArr(loc).locDom.low);
    randlist.fillRandom(C.locArr(loc).myElems);
  }

  if (printArrays) {
    writeln("B is: ", B, "\n");
    writeln("C is: ", C, "\n");
  }
}


def verifyResults(A, B, C) {
  if (printArrays) then writeln("A is: ", A, "\n");

  const infNorm = max reduce [i in A.dom] abs(A(i) - (B(i) + alpha * C(i)));

  return (infNorm <= epsilon);
}


def printResults(successful, execTimes) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    const totalTime = + reduce execTimes,
          avgTime = totalTime / numTrials,
          minTime = min reduce execTimes;
    writeln("Execution time:");
    writeln("  tot = ", totalTime);
    writeln("  avg = ", avgTime);
    writeln("  min = ", minTime);

    const GBPerSec = numVectors * numBytes(elemType) * (m / minTime) * 1e-9;
    writeln("Performance (GB/s) = ", GBPerSec);
  }
}

// TODO: Check diff with main stream.chpl to see what differs
