#include <SFML/Graphics.hpp>
#include <complex>
#include <thread>

const int W = 1600;
const int H = 1600;

double range = 2.0;
double ox = -1.5;
double oy = -1.0;

sf::Uint8* pixels = new sf::Uint8[W*H*4];
sf::Clock deltaClock;
sf::Text timeText;

void setPixelsToMandelbrot();
int getNumberOfIterations(std::complex<double> c, int maxIterations);
void updateMandelbrotPixels();

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(W, H), "My window");

    sf::Texture texture;
    texture.create(W, H);

    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(100, 100);
    timeText.setCharacterSize(24);
    sf::Font font;
    font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
    timeText.setFont(font);

    sf::Sprite sprite(texture);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Left) {
                    ox -= range * 0.1;
                }

                if(event.key.code == sf::Keyboard::Right) {
                    ox += range * 0.1;
                }

                if(event.key.code == sf::Keyboard::Up) {
                    oy -= range * 0.1;
                }

                if(event.key.code == sf::Keyboard::Down) {
                    oy += range * 0.1;
                }

            }
        }

//        range *= 0.99;

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        updateMandelbrotPixels();
        texture.update(pixels);
        window.draw(sprite);

        // show fps
        auto dt = deltaClock.getElapsedTime();
        deltaClock.restart();
        std::stringstream s;
        s<<dt.asSeconds();
        timeText.setString(s.str());

        window.draw(timeText);
        // end the current frame
        window.display();
    }

    return 0;
}

void setPixelsToMandelbrot(int a, int b) {


    for(int y=0; y<H; y++) {
        for(int x=0; x<W; x++) {
            int idx = ((y*W)+x)*4;


            double range_x = (((double) x/ (double) W))  * range;
            double range_y = (((double) y/ (double) H))  * range;

            std::complex<double> c;

            c.real(range_x + ox);
            c.imag(range_y + oy);


            int iters = getNumberOfIterations(c, 100);

            pixels[idx+0] = 255; // R
            pixels[idx+1] = floor(((double) iters/100.0)*255.0); // G
            pixels[idx+2] = 0; // B
            pixels[idx+3] = 255; // A
        }
    }
}

int getNumberOfIterations(std::complex<double> c, int maxIterations) {
    std::complex<double> z;
    z.real(0.0);
    z.imag(0.0);

    for(int i=0; i<maxIterations; i++) {
        z = (z*z) + c;

        double mag = (z.imag()*z.imag()) + (z.real()*z.real());

        if(mag > 4) {
            return i;
        }
    }

    return maxIterations;
}


void updateMandelbrotPixels() {
    const int noThreads = 1;
    std::thread threads[noThreads];

    for(int i=0; i<noThreads; i++) {
        threads[i] = std::thread(setPixelsToMandelbrot, i, noThreads);
    }

    for(int i=0; i<noThreads; i++) {
        threads[i].join();
    }
}