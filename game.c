#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;
ALLEGRO_SAMPLE* sample_gem;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* stone;
ALLEGRO_BITMAP* wall;
ALLEGRO_BITMAP* gem;
ALLEGRO_BITMAP* position[4];

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
    switch(event->type)
    {
        case ALLEGRO_EVENT_TIMER:
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}

void draw_map()
{
    int i, j;
    for(i=0; i<120; i+=3)
    {
        for(j=0; j<150; j+=3)
        {
            if(map[i][j] == '|')
                al_draw_scaled_bitmap(wall, 0, 0, 3, 3, j, i, 3, 3, 0);
            if(map[i][j] == 'R')
                al_draw_scaled_bitmap(stone, 0, 0, 3, 3, j, i, 3, 3, 0);
            if(map[i][j] == 'O')
                al_draw_scaled_bitmap(gem, 0, 0, 3, 3, j, i, 3, 3, 0);
            if(map[i][j] == 'X')
                al_draw_scaled_bitmap(position[player.pos], 0, 0, 3, 3, j, i, 3, 3, 0);
        }
    }
}

void draw_menu()
{
    al_draw_text(font, al_map_rgb_f(1,1,1), 100, 100, 0, "Mova-se com as setas do teclado e colete os diamantes");
    al_draw_text(font, al_map_rgb_f(1,1,1), 100, 120, 0, "Desenvolvido por Leonardo Goncalves");
}

void player_movement()
{
    int i, j;
    if(key[ALLEGRO_KEY_LEFT])
    {
        if(map[player.y][player.x-3] == '*')
        {
            for(i=player.y;i<player.y+3;i++)
                 for(j=player.x;j<player.x+3;j++)
                 {
                     map[i][j] = '*';
                     map[i][j-3] = 'X';
                 }
            player.x-=3;
        }
        if(map[player.y][player.x-3] == 'O')
        {
            for(i=player.y;i<player.y+3;i++)
                 for(j=player.x;j<player.x+3;j++)
                 {
                     map[i][j] = '*';
                     map[i][j-3] = 'X';
                 }
            al_play_sample(sample_gem, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            player.score += 500;
            player.gems_collected += 1;
            player.x-=3;   
        }
    }
    if(key[ALLEGRO_KEY_RIGHT])
    {
        if(map[player.y][player.x+3] == '*')
        {
            for(i=player.y;i<player.y+3;i++)
                 for(j=player.x;j<player.x+3;j++)
                 {
                     map[i][j] = '*';
                     map[i][j+3] = 'X';
                 }
            player.x+=3;
        }
        if(map[player.y][player.x+3] == 'O')
        {
            for(i=player.y;i<player.y+3;i++)
                 for(j=player.x;j<player.x+3;j++)
                 {
                     map[i][j] = '*';
                     map[i][j+3] = 'X';
                 }
            al_play_sample(sample_gem, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            player.score += 500;
            player.gems_collected += 1;   
            player.x+=3;
        }
    }
    if(key[ALLEGRO_KEY_UP])
    {
        if(map[player.y-3][player.x] == '*')
        {
            for(i=player.y;i<player.y+3;i++)
                 for(j=player.x;j<player.x+3;j++)
                 {
                     map[i][j] = '*';
                     map[i-3][j] = 'X';
                 }
            player.y-=3;
        }
        if(map[player.y-3][player.x] == 'O')
        {
            for(i=player.y;i<player.y+3;i++)
                 for(j=player.x;j<player.x+3;j++)
                 {
                     map[i][j] = '*';
                     map[i-3][j] = 'X';
                 }
            al_play_sample(sample_gem, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            player.score += 500;
            player.gems_collected += 1;   
            player.y-=3;
        }
    }
    if(key[ALLEGRO_KEY_DOWN])
    {
        if(map[player.y-3][player.x] == '*')
        {
            for(i=player.y;i<player.y+3;i++)
                 for(j=player.x;j<player.x+3;j++)
                 {
                     map[i][j] = '*';
                     map[i-3][j] = 'X';
                 }
            player.y+=3;
        }
        if(map[player.y+3][player.x] == 'O')
        {
            for(i=player.y;i<player.y+3;i++)
                 for(j=player.x;j<player.x+3;j++)
                 {
                     map[i][j] = '*';
                     map[i-3][j] = 'X';
                 }
            al_play_sample(sample_gem, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            player.score += 500;
            player.gems_collected += 1;
            player.y+=3;   
        }
    }
    if(key[ALLEGRO_KEY_C])
        player.score += 1000;
}

bool map_update()
{
    int i, j, y, x;
    for(i=3;i<117;i+=3)
        for(j=3;i<147;j+=3)
        {
            if(map[i][j] == 'R')
            {
                if(map[i+3][j] == '*')
                for(y=i;y<i+3;y++)
                    for(x=j;x<j+3;x++)
                    {
                        map[y][x] = '*';
                        queda[y][x] = 0;
                        map[y+3][x] = 'R';
                        queda[y+3][x] = 1;
                    }
            }
            if(map[i+3][j] == 'X' && queda[i][j] == 1)
            {
                player.life-=1;
                for(y=i;y<i+3;y++)
                    for(x=j;x<j+3;x++)
                    {
                        map[y][x] = '*';
                        map[y+3][x] = 'R';
                    }
                return true;
            }
            if(map[i+3][j] == 'R' && queda[i][j] == 0 && map[i][j-3] == '*')
            {
                for(y=i;y<i+3;y++)
                    for(x=j;x<j+3;x++)
                    {
                        map[y][x] = '*';
                        map[y][x-3] = 'R';
                    }
            }
            if(map[i+3][j] == 'R' && queda[i][j] == 0 && map[i][j+3] == '*')
            {
                for(y=i;y<i+3;y++)
                    for(x=j;x<j+3;x++)
                    {
                        map[y][x] = '*';
                        map[y][x+3] = 'R';
                    }
            }
        }
    return false;
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(4);
    sample_gem = al_load_sample("resources/coin.wav");
    must_init(sample_gem, "gem sample");
    font = al_create_builtin_font();
    must_init(font, "font");
    disp = al_create_display(1500, 1205);
    must_init(disp, "display");
    buffer = al_create_bitmap(150, 120);
    must_init(buffer, "bitmap buffer");
    font = al_create_builtin_font();
    must_init(font, "font");
    must_init(al_init_image_addon(), "image addon");
    stone = al_load_bitmap("resources/stone.png");
    must_init(stone, "stone");
    wall = al_load_bitmap("resources/wall.png");
    must_init(wall, "wall");
    ALLEGRO_BITMAP* gem = al_load_bitmap("resources/gem.png");
    must_init(gem, "gem");
    must_init(position, "position");
    position[0] = al_load_bitmap("resources/player0.jpg");
    position[1] = al_load_bitmap("resources/player1.jpg");
    position[2] = al_load_bitmap("resources/player2.jpg");
    position[3] = al_load_bitmap("resources/player3.jpg");
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    keyboard_init();
    int frames = 0;
    bool done = false;
    bool redraw = true;
    bool pause = false;
    ALLEGRO_EVENT event;
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(frames % 4 == 0 && !pause)
                {
                    player_movement();
                    done = map_update();
                    if(player.gems_collected = GEMS)
                        done = true;
                }
                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;
                if(key[ALLEGRO_KEY_H] || key[ALLEGRO_KEY_F1])
                    pause = !pause;
                redraw = true;
                frames++;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }
        if(done)
            break;
        keyboard_update(&event);
        if(redraw && al_is_event_queue_empty(queue))
        {
            al_set_target_bitmap(buffer);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_map();
            al_set_target_backbuffer(disp);
            al_draw_textf(font, al_map_rgb_f(1,1,1), 1, 1, 0, "%06d", player.score);
            al_draw_scaled_bitmap(buffer, 0, 0, 150, 120, 0, 5, 1500, 1200, 0);
            if(pause)
                draw_menu();
            al_flip_display();
            redraw = false;
        }
    }
    al_destroy_sample(sample_gem);
    al_destroy_font(font);
    al_destroy_bitmap(stone);
    al_destroy_bitmap(wall);
    al_destroy_bitmap(gem);
    al_destroy_bitmap(position[0]);
    al_destroy_bitmap(position[1]);
    al_destroy_bitmap(position[2]);
    al_destroy_bitmap(position[3]);
    al_destroy_bitmap(position[4]);
    al_destroy_bitmap(buffer);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    return 0;
}
