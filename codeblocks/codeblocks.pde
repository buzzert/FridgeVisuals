import java.util.Random;
import java.time.OffsetTime;
import java.time.temporal.ChronoUnit;

PFont font;

static final float viewportWidth = 64;
static final float viewportHeight = 160;

static String[] randomStrings = {
    "[+]",
    "[-]",
    "C6ORF60",
    "FAM9B",
    "1157KB",
    "CRXNI",
    "590KB-604KB",
    "SCRT2",
    "    ",
    " ",
    "  ",
    "       ",
    "XION",
    "+++",
    "LOC401959",
    "-"
};

static Random rand = new Random();
String randomString() {
    return randomStrings[rand.nextInt(randomStrings.length - 1)];
}

static ArrayList<ArrayList<TextLayer>> textLayerLines = new ArrayList<ArrayList<TextLayer>>();
void populateTextLayers() {
    float yPosition = 0;
    float xPosition = 0;

    int lineNumber = 0;
    final float layerHeight = 11.0;
    while (yPosition < viewportWidth) {
        ArrayList<TextLayer> currentLine;
        if (lineNumber > (textLayerLines.size() - 1)) {
            currentLine = new ArrayList<TextLayer>();
            textLayerLines.add(currentLine);
        } else {
            currentLine = textLayerLines.get(lineNumber);
            TextLayer lastTextLayer = currentLine.get(currentLine.size() - 1);
            xPosition = lastTextLayer.pos.x + lastTextLayer.getWidth();
        }

        while (xPosition < viewportHeight) {
            String randomString = randomString();
            TextLayer t = new TextLayer(new Position(xPosition, yPosition), randomString, font, 24.0);
            t.layerHeight = layerHeight;
            currentLine.add(t);

            boolean hasBackground = rand.nextBoolean();
            if (hasBackground) {
                t.backgroundColor = new Color(255, 255, 255, 255);
                t.foregroundColor = new Color(0, 0, 0, 255);
            }

            boolean flashes = rand.nextBoolean();
            flashes &= rand.nextBoolean();
            if (flashes) {
                t.flashDuration = 0.4 + (rand.nextFloat() % 3.0);
            }

            if (randomString == "XION") {
                t.backgroundColor = new Color(0, 169, 255, 255);
                t.foregroundColor = new Color(0, 0, 0, 255);
            }

            xPosition += t.getWidth();
        }

        xPosition = 0.0;
        yPosition += layerHeight;
        lineNumber++;
    }
}

static OffsetTime lastRainbowTime = OffsetTime.now();
static boolean rainbowToBlackness = true;
static float rainbowWindowPosition = viewportHeight;
void drawRainbowEffect() {
    if (rainbowWindowPosition < -viewportHeight) {
        // Start the wipe again, but with blackness set to FALSE so the layers
        // get their "normal" background color again.
        rainbowToBlackness = false;
        rainbowWindowPosition = viewportHeight;
    }

    for (ArrayList<TextLayer> line : textLayerLines) {
        for (TextLayer layer : line) {
            float xPosition = layer.pos.x;

            // Layers that are within the window position...
            if ( Math.abs(xPosition - rainbowWindowPosition) < 10.0 ) {
                if (rainbowToBlackness) {
                    // and if to blackness is TRUE, set it to it's corresponding rainbow color
                    float hueValue = (xPosition / viewportHeight);

                    int rgbVal = java.awt.Color.HSBtoRGB(hueValue, 1.0, 1.0);
                    java.awt.Color colorVal = new java.awt.Color(rgbVal);
                    layer.backgroundColor = new Color(colorVal.getRed(), colorVal.getGreen(), colorVal.getBlue(), 255);
                    layer.foregroundColor = new Color(0, 0, 0, 255);
                } else {
                    // if blackness is FALSE, set it to a "normal" color (i.e., black on white)
                    layer.backgroundColor = new Color(255, 255, 255, 255);
                    layer.foregroundColor = new Color(0, 0, 0, 255);
                }
            } else {
                // If the layer is outside of the window position
                if (rainbowToBlackness) {
                    // and BLACKNESS is true, set it to BLACKNESS
                    layer.backgroundColor = new Color(0, 0, 0, 255);
                    layer.foregroundColor = new Color(255, 255, 255, 0);
                }
            }
        }
    }

    rainbowWindowPosition -= 4.0;
}

void startRainbowEffect() {
    lastRainbowTime = OffsetTime.now();
    rainbowToBlackness = true;
    rainbowWindowPosition = viewportHeight;
}

void updateRainbowEffectState() {
    final long rainbowTimeEffectDuration = 5;

    if (lastRainbowTime.until(OffsetTime.now(), ChronoUnit.SECONDS) < rainbowTimeEffectDuration) {
        drawRainbowEffect();
    }

    final long rainbowTimeEffectPeriod = 25;
    if (lastRainbowTime.until(OffsetTime.now(), ChronoUnit.SECONDS) > rainbowTimeEffectPeriod) {
        startRainbowEffect();
    }
}

void updateTextLayers() {
    final float velocityMultiplier = 0.04;
    final float[] lineVelocities = {
        5.0,
        11.0,
        15.0,
        10.0,
        8.0,
        9.0,
        13.0,
        18.0
    };

    for (int lineNum = 0; lineNum < textLayerLines.size(); lineNum++) {
        float velocity = velocityMultiplier * lineVelocities[lineNum % lineVelocities.length];

        ArrayList<TextLayer> layersToRemove = new ArrayList<TextLayer>();
        ArrayList<TextLayer> currentLine = textLayerLines.get(lineNum);
        for (TextLayer layer : currentLine) {
            layer.pos.x -= velocity;

            // cull layers that are off screen
            if ( (layer.pos.x + layer.getWidth()) < 0 ) {
                layersToRemove.add(layer);
            }
        }

        for (TextLayer layer : layersToRemove) {
            currentLine.remove(layer);
        }
    }
}

void setup() {
    fullScreen(P3D);
    noSmooth(); // disable anti-aliasing
    //size(64, 160, P3D);

    font = loadFont("Heytext-24.vlw");

    // Camera will be oriented 90deg clockwise, so the eyeX and eyeY are based on 
    // height and width respectively, instead of width and height. 

    // For some reason, this doesn't work with `fullScreen()`...
    /*
    float cameraEyeX = width / 2.0; //height / 2.0;
    float cameraEyeY = height / 2.0; //width / 2.0;
    camera(
        cameraEyeX,     // eyeX
        cameraEyeY,     // eyeY
        cameraEyeX / tan(PI*30.0 / 180.0), // eyeZ

        cameraEyeX,      // centerX
        cameraEyeY,      // centerY
        0,               // centerZ
        1,               // upX
        0,               // upY
        0                // upZ
    );
*/
}

void draw() {
    background(0);
    fill(255);

    populateTextLayers();
    
    // Convoluted way of getting the viewport on the top left corner.
    // Need to research a better way to do this, since it seems `camera` doesn't work in fullscreen.
    translate(width/2, height/2);
    rotate(PI/2);
    translate(-height/2, -width/2);
    translate(0.0, width - viewportWidth);
    
    for (ArrayList<TextLayer> line : textLayerLines) {
        for (TextLayer layer : line) {
            layer.draw();
        }
    }

    updateTextLayers();
    updateRainbowEffectState();
}
