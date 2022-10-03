This is code I added to try to make the message area transparent - i.e. the message window would still show the background garphics rather than a blank white. It halfway worked - the problem is with the text. The Windows GDI doesn't provide a way to draw text without drawing the background.

Shawn Overcash