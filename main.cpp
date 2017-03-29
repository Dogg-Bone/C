#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <MATH.h>

#define WIDE 800
#define HIGH 640
#define TILESIZE 32

bool testColl(sf::Sprite a, sf::Sprite b)
{
  return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

bool testWallColl(sf::Sprite a, std::vector<sf::Sprite> walls)
{
  for(int i=0; i<walls.size(); i++)
  {
    if(a.getGlobalBounds().intersects(walls[i].getGlobalBounds()))
    {
      return true;
    }
  }
  return false;
}



int main()
{
  //declare and create the things
  sf::RenderWindow window(sf::VideoMode(WIDE,HIGH),"Once upon a time there was a Boy named Timothy");

  int tileW = WIDE/TILESIZE;
  int tileH = HIGH/TILESIZE;

  std::vector<std::vector<bool> > map;

  for(int i=0; i<tileH; i++)
  {
    std::vector<bool> temp;
    for(int j=0; j<tileW; j++)
    {
      temp.push_back(false);
    }
    map.push_back(temp);
  }

  map[3][6] = true;
  map[4][6] = true;
  map[5][6] = true;
  map[6][6] = true;
  map[7][6] = true;
  map[8][6] = true;
  map[3][7] = true;
  map[3][8] = true;
  map[3][9] = true;
  map[3][10] = true;
  map[3][11] = true;
  map[3][12] = true;
  map[8][7] = true;
  map[8][8] = true;
  map[8][9] = true;
  map[8][10] = true;
  map[8][11] = true;
  map[8][12] = true;
  map[3][12] = true;
  map[4][12] = true;
  map[7][12] = true;
  map[8][12] = true;
  map[8][15] = true;
  map[8][16] = true;
  map[8][17] = true;
  map[8][18] = true;
  map[7][18] = true;
  map[7][15] = true;
  map[6][18] = true;



  for(int i=0; i<tileH; i++)
  {
    map[i][0] = true;
    map[i][tileW-1] = true;
  }
  for(int i=0; i<tileW; i++)
  {
    map[0][i] = true;
    map[tileH-1][i] = true;
  }


  for(int i=0; i<map.size(); i++)
  {
    for(int j=0; j<map[i].size(); j++)
    {
      if(map[i][j] == true)
      {
        std::cout << 'O';
      }
      else
      {
        std::cout << ' ';
      }
    }
    std::cout << std::endl;
  }

  //assets and such
  sf::Font font;
  if(!font.loadFromFile("Assets/ARCADECLASSIC.TTF"))
  {
    std::cout << "Error Loading Font" << std::endl;
  }
  sf::Text livesDisp;
  livesDisp.setPosition(760,0);
  livesDisp.setFont(font);
  livesDisp.setCharacterSize(24);
  livesDisp.setColor(sf::Color::Red);

  sf::SoundBuffer grunt_buffer;
  if(!grunt_buffer.loadFromFile("Assets/grunt.wav"))
  {
    std::cout << "Error loading grunt.wav.";
  }

  sf::SoundBuffer music_buffer;
  if(!music_buffer.loadFromFile("Assets/Ninja Flute-Lolita Compiex.wav"))
  {
    std::cout << "Error loading theme song.";
  }

  sf::Texture hero_texture;
  if(!hero_texture.loadFromFile("Assets/hero.png"))
  {
    std::cout << "Error loading hero texture.";
  }

  sf::Texture wall_texture;
  if(!wall_texture.loadFromFile("Assets/wall.png"))
  {
    std::cout << "Error Loading Texture" << std::endl;
  }

  sf::Texture enemy_texture;
  if(!enemy_texture.loadFromFile("Assets/skelly.png"))
  {
    std::cout << "Error Loading Texture" << std::endl;
  }

  sf::Texture homing_enemy_texture;
  if(!homing_enemy_texture.loadFromFile("Assets/ghost.png"))
  {
    std::cout << "Error Loading Texture" << std::endl;
  }

  sf::Texture stone_texture;
  if(!stone_texture.loadFromFile("Assets/stone.png"))
  {
    std::cout << "Error Loading Texture" << std::endl;
  }

  sf::Sound grunt;
  grunt.setBuffer(grunt_buffer);
  grunt.setPitch(1);

  sf::Sound music;
  music.setBuffer(music_buffer);
  music.setLoop(true);

  sf::Sprite enemy;
  enemy.setScale(sf::Vector2f(3,3));
  enemy.setTexture(enemy_texture);
  enemy.setPosition(TILESIZE*4,TILESIZE*4);

  sf::Sprite homing_enemy;
  homing_enemy.setScale(sf::Vector2f(2,2));
  homing_enemy.setTexture(homing_enemy_texture);
  homing_enemy.setPosition(TILESIZE*6,TILESIZE*6);

  sf::Sprite hero;
  hero.setTexture(hero_texture);
  hero.setScale(sf::Vector2f(2,2));
  hero.setPosition(TILESIZE*2,TILESIZE*2);


  std::vector<sf::Sprite> wallArr;
  for(int i=0; i<map.size(); i++)
  {
    for(int j=0; j<map[i].size(); j++)
    {
      if(map[i][j] == true)
      {
        sf::Sprite temp;
        temp.setTexture(wall_texture);
        temp.setScale(sf::Vector2f(2,2));
        temp.setPosition(j*TILESIZE,i*TILESIZE);
        wallArr.push_back(temp);
      }
    }
  }

  sf::Sprite stone;
  stone.setTexture(stone_texture);
  stone.setScale(sf::Vector2f(2,2));
  stone.setPosition(0,0);

  //Variables and such
  int enemyDir = 0;

  double heroSpeed = 0.5;

  double homingX = 0.5;
  double homingY = 0.5;
  double homingDist = 0;

  int lives = 5;

  bool takingDamage = false;

  music.play();


  int circleDirection = 0;

  int points = 0;

  //Game Loop
  while(window.isOpen())
  {
    //Handle Only Certain Events
    sf::Event event;
    while(window.pollEvent(event))
    {
      //Close Event
      if(event.type == sf::Event::Closed)
      {
        window.close();
      }

    }
    //Handle Input NOW!!!
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && hero.getPosition().y > 0)
    {
      hero.move(0,-heroSpeed);
      if(testWallColl(hero, wallArr))
      {
        hero.move(0,heroSpeed);
      }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && hero.getPosition().y < HIGH-TILESIZE)
    {
      hero.move(0,heroSpeed);
      if(testWallColl(hero, wallArr))
      {
        hero.move(0,-heroSpeed);
      }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && hero.getPosition().x > 0)
    {
      hero.move(-heroSpeed,0);
      if(testWallColl(hero, wallArr))
      {
        hero.move(heroSpeed, 0);
      }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && hero.getPosition().x < WIDE-TILESIZE)
    {
      hero.move(heroSpeed,0);
      if(testWallColl(hero, wallArr))
      {
        hero.move(-heroSpeed, 0);
      }
    }



    window.clear(sf::Color::White);
    if(enemyDir == 0)
    {
      enemy.move(1,0);
      if(testWallColl(enemy, wallArr))
      {
        enemyDir = 1;
        enemy.move(-1,0);
      }
    }
    else
    {
      enemy.move(-1,0);
      if(testWallColl(enemy, wallArr))
      {
        enemyDir = 0;
        enemy.move(1,0);
      }
    }
    if(enemy.getPosition().x > 800)
    {
      enemyDir = 1;
    }
    if(enemy.getPosition().x < 0)
    {
      enemyDir = 0;
    }

    homingDist = sqrt(pow(hero.getPosition().x - homing_enemy.getPosition().x, 2) + pow(hero.getPosition().y - homing_enemy.getPosition().y, 2));
    homingX = (hero.getPosition().x - homing_enemy.getPosition().x)/homingDist;
    homingY = (hero.getPosition().y - homing_enemy.getPosition().y)/homingDist;
    homing_enemy.move(homingX/4, homingY/4);

    if(hero.getGlobalBounds().intersects(enemy.getGlobalBounds()))
    {
      if(takingDamage == false)
      {
        lives--;
        grunt.play();
        takingDamage = true;
      }
    }
    else
    {
      takingDamage = false;
    }

    livesDisp.setString(std::to_string(lives));

    //RENDER THE SCREEN!
      for(int i=0; i<tileH; i++)
      {
        for(int j=0; j<tileW; j++)
        {
          stone.setPosition(j*TILESIZE, i*TILESIZE);
          window.draw(stone);
        }
      }

    for(int i=0; i<wallArr.size(); i++)
    {
      window.draw(wallArr[i]);
    }
    window.draw(homing_enemy);
    window.draw(hero);
    window.draw(enemy);
    window.draw(livesDisp);
    window.display();

  }
}
