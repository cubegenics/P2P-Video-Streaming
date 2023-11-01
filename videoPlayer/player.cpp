#include <SFML/Graphics.hpp>
#include <vlc/vlc.h>
#include <cassert>
#include <unistd.h>

#define WIDTH 1366
#define HEIGHT 768

using namespace sf;

struct ctx
{
    Uint8 *surf;
};

static void *e_lock(void *data, void **p_pixels)
{
    struct ctx *ctx = (struct ctx*)data;
    *p_pixels = ctx;
    return NULL;
}

static void unlock(void *data, void *id, void *const *p_pixels)
{
    struct ctx *ctx = (struct ctx*)data;

    assert(id == NULL);
}

static void display(void *data, void *id)
{
    (void) data;
    assert(id == NULL);
}

void SFML_VLC(char* filename)
{
    sf::VideoMode VMode(1366, 768, 16);
    sf::RenderWindow Window(VMode, "SFML 2", sf::Style::Close);

    libvlc_instance_t *libvlc = NULL;
    libvlc_media_t *m = NULL;
    libvlc_media_player_t *mp = NULL;

    char const *vlc_arg[] =
    {
        "--no-video-title-show"
    };
    int count_arg = sizeof(vlc_arg) / sizeof(*vlc_arg);
    libvlc = libvlc_new(count_arg, vlc_arg);
   // char filename[] = "abc.mp4";
    m = libvlc_media_new_path(libvlc, (std::string("../client/") + filename).c_str());
    mp = libvlc_media_player_new_from_media(m);
    libvlc_media_release(m);

    Uint8 e_frame[WIDTH * HEIGHT * 4];

    libvlc_video_set_callbacks(mp, e_lock, unlock, display, &e_frame);
    libvlc_video_set_format(mp, "RGBA", WIDTH, HEIGHT, WIDTH * 4);
    libvlc_media_player_play(mp);

    Texture texture;
    texture.create(WIDTH, HEIGHT);
    Sprite sprite;
    sprite.setTexture(texture);

    while(Window.isOpen())
    {
        sf::Event Event;
        while(Window.pollEvent(Event))
        {
            switch(Event.type)
            {
                case sf::Event::Closed:
                    Window.close();
                    break;
                default:
                    break;
            }
        }

        Window.clear();
        texture.update(e_frame);
        Window.draw(sprite);
        Window.display();

    }

    libvlc_media_player_stop(mp);
    libvlc_media_player_release(mp);
    libvlc_release(libvlc);
}

int main(int argc, char** argv)
{
    sleep(8);
    SFML_VLC(argv[0]);
}