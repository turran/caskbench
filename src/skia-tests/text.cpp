// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include <SkCanvas.h>
#include <SkPaint.h>
#include <cairo.h>

#include "caskbench.h"
#include "SkGpuDevice.h"
#include "SkString.h"
#include "SkTemplates.h"
#include "SkTypeface.h"
#include "effects/SkGradientShader.h"
#include "unistd.h"
#include "SkUnitMappers.h"
#include "SkDraw.h"
#include "effects/SkGradientShader.h"
#include "SkGraphics.h"
static int max_dim;

static char rand_text_array[19][100];
void gen_skia_random(char *s, const int len) {
    static const char alphanum[] =
        "ABCD EFG HIJKL MNOPQ RSTUVW XYZ";
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[len] = 0;
}

	int
sk_setup_text(caskbench_context_t *ctx)
{
    max_dim = MIN(ctx->canvas_width, ctx->canvas_height)/2;
	for(int i = 0;i<19;i++)
	{
		gen_skia_random(rand_text_array[i],i+18);
	}
    return 1;
}

	void
sk_teardown_text(void)
{
}

static void getGlyphPositions(const SkPaint& paint, const uint16_t glyphs[],
                             int count, SkScalar x, SkScalar y, SkPoint pos[]) {
    SkASSERT(SkPaint::kGlyphID_TextEncoding == paint.getTextEncoding());

    SkAutoSTMalloc<128, SkScalar> widthStorage (count);
    SkScalar* widths = widthStorage.get();
    paint.getTextWidths (glyphs, count * sizeof(uint16_t), widths);

    for (int i = 0; i < count; ++i) {
        pos[i].set(x, y);
        x += widths[i];
    }
}



	int
sk_test_text(caskbench_context_t *ctx)
{
    unsigned char red, green, blue, alpha;
    for (int i = 0; i < ctx->size; i++)
    {
        double font_size = 18;
        int ypos = 0, xpos = 0;
        ctx->skia_canvas->drawColor(SK_ColorBLACK);

        for(font_size = 18; font_size <= 36; font_size++) 
		{
		    ctx->skia_paint->setTextSize (SkIntToScalar (font_size));
            red = int( 255 * (double)rand()/RAND_MAX );
            green = int( 255 * (double)rand()/RAND_MAX );
            blue = int( 255 * (double)rand()/RAND_MAX );
            alpha = int( 255 * (double)rand()/RAND_MAX );
            ctx->skia_paint->setARGB (alpha,red,green,blue);

			SkPaint newpaint (*ctx->skia_paint);;
    		newpaint.setTextEncoding (SkPaint::kGlyphID_TextEncoding);

    		SkAutoSTMalloc<128, uint16_t> glyphStorage (font_size);
		    uint16_t* glyphs = glyphStorage.get();

            char text[(int)font_size];
            gen_skia_random (text,font_size);

		    int glyphCount = ctx->skia_paint->textToGlyphs (text, font_size, glyphs);
		    ctx->skia_canvas->drawText (glyphs, glyphCount * sizeof(uint16_t), xpos,ypos, newpaint);
			ypos += (font_size/2);
		}

        for(font_size = 36; font_size >= 18; font_size--)
        {
            ctx->skia_paint->setTextSize (SkIntToScalar (font_size));
            red = int( 255 * (double)rand()/RAND_MAX );
            green = int( 255 * (double)rand()/RAND_MAX );
            blue = int( 255 * (double)rand()/RAND_MAX );
            alpha = int( 255 * (double)rand()/RAND_MAX );
            ctx->skia_paint->setARGB (alpha,red,green,blue);
	
			SkPaint localPaint (*ctx->skia_paint);;
			localPaint.setTextEncoding (SkPaint::kGlyphID_TextEncoding);

		    SkAutoSTMalloc<128, uint16_t> glyphStorage (font_size);
			uint16_t* glyphs = glyphStorage.get();
            char text[(int)font_size];
            gen_skia_random (text,font_size);
		    
			int glyphCount = ctx->skia_paint->textToGlyphs (text, font_size, glyphs);
		    ctx->skia_canvas->drawText (glyphs, glyphCount * sizeof(uint16_t), xpos,ypos, localPaint);
            ypos += (font_size/2);
    	}

	}	
    return 1;
}
