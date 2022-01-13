#include <GL/glew.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
enum dds_header_flags {
    DDS_FLAGS_CAPS = 0x1,
    DDS_FLAGS_HEIGHT = 0x2,
    DDS_FLAGS_WIDTH = 0x4,
    DDS_FLAGS_PITCH = 0x8,
    DDS_FLAGS_PIXELFORMAT = 0x1000,
    DDS_FLAGS_MIPMAPCOUNT = 0x20000,
    DDS_FLAGS_LINEARSIZE = 0x80000,
    DDS_FLAGS_DEPTH = 0x800000
};

enum dds_caps1_flags {
    DDS_CAPS1_COMPLEX = 0x8,
    DDS_CAPS1_MIPMAP = 0x400000,
    DDS_CAPS1_TEXTURE = 0x1000
};

enum dds_caps2_flags {
    DDS_CAPS2_CUBEMAP = 0x200,
    DDS_CAPS2_CUBEMAP_POSITIVEX = 0x400,
    DDS_CAPS2_CUBEMAP_NEGATIVEX = 0x800,
    DDS_CAPS2_CUBEMAP_POSITIVEY = 0x1000,
    DDS_CAPS2_CUBEMAP_NEGATIVEY = 0x2000,
    DDS_CAPS2_CUBEMAP_POSITIVEZ = 0x4000,
    DDS_CAPS2_CUBEMAP_NEGATIVEZ = 0x8000,
    DDS_CAPS2_VOLUME = 0x200000,
};

enum dds_pixel_format_flags {
    DDS_PF_ALPHAPIXELS = 0x1,
    DDS_PF_ALPHA = 0x2,
    DDS_PF_FOURCC = 0x4,
    DDS_PF_RGB = 0x40,
    DDS_PF_YUV = 0x200,
    DDS_PF_LUMINANCE = 0x20000
};

enum dds_image_formats {
    DDS_IMAGE_FORMAT_UNKNOWN,
    DDS_IMAGE_FORMAT_DXT1,
    DDS_IMAGE_FORMAT_DXT3,
    DDS_IMAGE_FORMAT_DXT5
};

typedef struct {
    unsigned int size;
    unsigned int flags;
    char fourCC[4];
    unsigned int rgbBitCount;
    unsigned int rBitMask;
    unsigned int gBitMask;
    unsigned int bBitMask;
    unsigned int aBitMask;
} dds_pixel_format;

typedef struct {
    unsigned int caps1;
    unsigned int caps2;
    unsigned int reserved[2];
} dds_caps2;

typedef struct {
    unsigned int size;
    unsigned int flags;
    unsigned int height;
    unsigned int width;
    unsigned int pitchOrLinearSize;
    unsigned int depth;
    unsigned int mipMapCount;
    unsigned int reserved[11];
    dds_pixel_format pixel_format;
    dds_caps2 caps;
    unsigned int reserved2;
} dds_header;

typedef struct {
    unsigned int w;
    unsigned int h;
    unsigned int buffer_size;
    char* buffer;
} dds_layer;

typedef struct {
    dds_layer* layers;
    unsigned int layers_count;
    int format;
} dds_image;

#define HAS_FLAG(X, Y) ((X & Y) > 0)
#define MAX(X, Y) X > Y ? X : Y

dds_image* load_dds_image(const char* f) {
    FILE* file;
    if ((file = fopen(f, "rb")) == 0) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char magik[4];
    fread(magik, 1, 4, file);

    if (memcmp(magik, "DDS ", 4) != 0) {
        fclose(file);
        return NULL;
    }

    dds_header header;
    fread(&header, 1, sizeof(dds_header), file);

    dds_image* img = (dds_image*)malloc(sizeof(dds_image));

    unsigned int w = header.width;
    unsigned int h = header.height;
    unsigned int block_size = 0;
    unsigned int offset = 0;
    if (header.pixel_format.fourCC[0] == 'D') {
        switch (header.pixel_format.fourCC[3]) {
        case '1': img->format = DDS_IMAGE_FORMAT_DXT1; block_size = 8; break;
        case '3': img->format = DDS_IMAGE_FORMAT_DXT3; block_size = 16; break;
        case '5': img->format = DDS_IMAGE_FORMAT_DXT5; block_size = 16; break;
        default: img->format = DDS_IMAGE_FORMAT_UNKNOWN; break;
        }
        if (img->format == DDS_IMAGE_FORMAT_UNKNOWN) {
            goto fail;
        }
    } else {
        goto fail;
    }

    if ( !HAS_FLAG(header.flags ,DDS_FLAGS_MIPMAPCOUNT) ) {
        goto fail;
    }

    img->layers = (dds_layer*)malloc(sizeof(dds_layer) * header.mipMapCount);
    img->layers_count = header.mipMapCount;

    for(unsigned int i = 0; i < header.mipMapCount; ++i) {
        unsigned int size = ((w + 3) / 4) * ((h + 3) / 4) * block_size;
        img->layers[i].buffer_size = size;
        img->layers[i].buffer = (char*)malloc(size);
        fread(img->layers[i].buffer, 1, size, file);
        img->layers[i].h = h;
        img->layers[i].w = w;
        w = MAX(1, w / 2);
        h = MAX(1, h / 2);
    }
    return img;
fail:
    free(img);
    fclose(file);
    return NULL;
}

unsigned int glLoadDDSTexture(const char* f) {
    dds_image* img = load_dds_image(f);
    if (!img) {
        return 0;
    }

    unsigned int gl_format;
    switch (img->format) {
    case DDS_IMAGE_FORMAT_DXT1: gl_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
    case DDS_IMAGE_FORMAT_DXT3: gl_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
    case DDS_IMAGE_FORMAT_DXT5: gl_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
    default:
        free(img);
        return 0;
    }

    unsigned int gl_tex_id = 0;
    glGenTextures(1, &gl_tex_id);
    if (gl_tex_id == 0) {
        free(img);
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, gl_tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, img->layers_count - 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    for(unsigned int i = 0; i < img->layers_count; ++i) {
        dds_layer* layer = &img->layers[i];
        glCompressedTexImage2D(GL_TEXTURE_2D, i, gl_format, img->layers[i].w, img->layers[i].h, 0, img->layers[i].buffer_size, img->layers[i].buffer);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, img->layers_count - 1);
    glBindTexture(GL_TEXTURE_2D, 0);

    free(img);
    return gl_tex_id;
}
