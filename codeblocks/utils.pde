import java.time.OffsetTime;
import java.time.temporal.ChronoUnit;

class Color {
    float red;
    float green;
    float blue;
    float alpha;

    public Color(float red, float green, float blue, float alpha) {
        this.red = red;
        this.green = green;
        this.blue = blue;
        this.alpha = alpha;
    }

    public void setFill() {
        fill(this.red, this.green, this.blue, this.alpha);
    }
}

class Position {
    float x;
    float y;

    public Position(float x, float y) {
        this.x = x;
        this.y = y;
    }
}

private class TextLayer {
    static final float horizontalPadding = 2;

    public Position pos;
    public Color backgroundColor;
    public Color foregroundColor;
    public String text;
    public PFont font;
    public float fontSize;
    public float layerHeight;
    public float flashDuration;

    private boolean flashOn = false;
    private OffsetTime lastFlashTime = OffsetTime.now();

    public TextLayer(Position pos, String text, PFont font, float fontSize) {
        this.pos = pos;
        this.text = text;
        this.font = font;
        this.fontSize = fontSize;
        this.layerHeight = fontSize;

        this.backgroundColor = new Color(0, 0, 0, 255);
        this.foregroundColor = new Color(255, 255, 255, 255);
    }

    public void draw() {
        textFont(this.font, this.fontSize);

        final float myWidth = this.getWidth();

        this.backgroundColor.setFill();
        rect(this.pos.x, this.pos.y, myWidth, this.layerHeight);

        final float becauseThisFontHasWeirdDimensions = 1;
        this.foregroundColor.setFill();
        text(this.text, this.pos.x + horizontalPadding, this.pos.y + this.layerHeight - becauseThisFontHasWeirdDimensions);

        this.updateBackgroundState();
    }

    public float getWidth() {
        textFont(this.font, this.fontSize);
        return textWidth(this.text) + (horizontalPadding * 2.0);
    }

    private void toggleForegroundAndBackgroundColors() {
        Color bgColor = this.backgroundColor;
        this.backgroundColor = this.foregroundColor;
        this.foregroundColor = bgColor;
    }

    // For flashing
    public void updateBackgroundState() {
        if (this.flashDuration == 0.0) {
            return;
        }

        OffsetTime now = OffsetTime.now();

        long duration = (long)(1000 * this.flashDuration);
        if (this.flashOn) {
            // Stay on for half as long as off.
            duration = duration / 2;
        }

        if (this.lastFlashTime.until(now, ChronoUnit.MILLIS) > duration) {
            this.toggleForegroundAndBackgroundColors();
            
            this.lastFlashTime = now;
            this.flashOn = !this.flashOn;
        }
    }
}