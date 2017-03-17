use OwnedObject;

class C {
  var x:int;
  proc ~C() {
    writeln("Destroying C x=", x);
  }
}

proc takeOwnershipAgain(in arg:Owned(C)) {
  writeln("in takeOwnershipAgain with arg=", arg.borrow());
}


proc takeOwnership(in arg:Owned(C)) {
  writeln("in takeOwnership with arg=", arg.borrow());
  takeOwnershipAgain(arg);
  writeln("in takeOwnership, now arg=", arg.borrow());
}


proc make() {
  var x = new Owned(new C(1));
  return x;
}


proc foo() {
  var x = make();

  takeOwnership(x);
}


foo();

