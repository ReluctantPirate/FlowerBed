enum flowerPhases {SEED, STALK, BUD, BLOOM};
byte flowerPhase = SEED;

Color flowerColors[4] = {RED, ORANGE, YELLOW, BLUE};
byte flowerColor = 0;
bool pointsScored = false;
bool celebrateAnyway = false;

void setup() {
  randomize();
}

void loop() {
  if (buttonSingleClicked()) {
    flowerPhase = (flowerPhase + 1) % 4;
    if (flowerPhase == BUD) {
      flowerColor = random(3);
    }
  }

  if (buttonMultiClicked()) {
    flowerPhase = SEED;
  }

  checkPoints();

  displayFlower();

  setValueSentOnAllFaces((pointsScored << 4) + (flowerPhase << 2) + flowerColor);
}

void checkPoints() {
  pointsScored = false;
  celebrateAnyway = false;
  byte totalNeighbors = 0;
  byte budsFound = 0;
  byte bloomsFound = 0;
  bool flowerbedColors[4] = {false, false, false, false};

  //first, count myself
  if (flowerPhase == BUD) {
    budsFound++;
  } else if (flowerPhase == BLOOM) {
    bloomsFound++;
  }

  flowerbedColors[flowerColor] = true;

  //count the neighbors
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//neighbor!
      totalNeighbors++;
      if (getFlowerPhase(getLastValueReceivedOnFace(f)) == BUD) {
        budsFound++;
        flowerbedColors[getFlowerColor(getLastValueReceivedOnFace(f))] = true;
      } else if (getFlowerPhase(getLastValueReceivedOnFace(f)) == BLOOM) {
        bloomsFound++;
        flowerbedColors[getFlowerColor(getLastValueReceivedOnFace(f))] = true;
      }

      //also ask if they are celebrating
      if (getPointsScored(getLastValueReceivedOnFace(f)) == true) {
        celebrateAnyway = true;
      }
    }
  }

  //here's what I'm interested in
  //do I have only 2 neighbors? That's the only way to get points
  if (totalNeighbors == 2) {
    //ok, so I have 2 neighbors. Let's see what our overall composition is
    if ((budsFound + bloomsFound) == 3) {//we have 3 buds/blooms!
      //count how many colors are in the bed
      byte colorsFound = 0;
      for (byte i = 0; i < 4 ; i++) {
        if (flowerbedColors[i] == true) {
          colorsFound++;
        }
      }

      //are there the right amount?
      if (colorsFound == 0) {
        pointsScored = true;
      } else if (colorsFound == 3) {
        pointsScored = true;
      }
    }
  }

}

void displayFlower() {
  if (pointsScored || celebrateAnyway) {
    setColor(dim(WHITE, random(100)));
  } else {
    setColor(OFF);
  }

  setColorOnFace(GREEN, 0);
  if (flowerPhase == STALK) {
    setColorOnFace(GREEN, 3);
  } else if (flowerPhase == BUD) {
    setColorOnFace(flowerColors[flowerColor], 3);
  } else if (flowerPhase == BLOOM) {
    setColorOnFace(flowerColors[flowerColor], 2);
    setColorOnFace(WHITE, 3);
    setColorOnFace(flowerColors[flowerColor], 4);
  }
}

byte getFlowerPhase(byte data) {
  return ((data >> 2) & 3);
}

byte getFlowerColor(byte data) {
  return (data & 3);
}

byte getPointsScored(byte data) {
  return ((data >> 4) & 3);
}
