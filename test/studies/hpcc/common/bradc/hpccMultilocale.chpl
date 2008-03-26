var myTurn$: [LocaleSpace] sync bool;
myTurn$(0) = true;


def waitForTurn() {
  myTurn$(localeID());
}


def passTurn() {
  myTurn$((localeID() + 1) % numLocales) = true;
  if (localeID() == 0) then {
    myTurn$(localeID());
    myTurn$(localeID()) = true;
  }
}


def writelnFragArray(startStr, X, endStr) {
  if (localeID() == 0) {
    write(startStr);
  }
  waitForTurn();
  if (localeID() != 0 && X.numElements != 0) {
    write(" ");
  }
  write(X);
  passTurn();
  if (localeID() == 0) {
    writeln(endStr);
  }
}


def BlockPartition(D, myBlock, numBlocks) {
  const lo = D.dim(1)._low;
  const hi = D.dim(1)._high;
  const numelems = hi - lo + 1;
  const nbAsIndexType = numBlocks:index(D);
  const myBlockAsIndexType:index(D) = myBlock;
  const blo = lo + (myBlockAsIndexType*numelems) / nbAsIndexType;
  const bhi = lo + (((myBlockAsIndexType+1)*numelems) / nbAsIndexType) - 1;
  return [blo..bhi];
}
