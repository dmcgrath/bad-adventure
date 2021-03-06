/* 
 * File:   world.cc
 * Author: itcmcgrath
 * 
 * Created on February 16, 2014, 10:21 PM
 * 
 * A quick overview of all the classes can be found in the header file.
 * 
 */

#include <string.h>

#include "world.h"

Direction::Direction() {
}

Direction::Direction(char direction) {
  this->direction = direction;
  switch (direction) {
    case 'n':
      description = "A path to the North";
      break;
    case 's':
      description = "A path to the South";
      break;
    case 'e':
      description = "A path to the East";
      break;
    case 'w':
      description = "A path to the West";
      break;
    case 'u':
      description = "A ladder headed up";
      break;
    case 'd':
      description = "A man-sized hole in the ground";
  }
}

Direction::Direction(char direction, std::string description) {
  this->direction = direction;
  this->description = description;
}

Direction::Direction(Direction &direction) {
  this->description = direction.description;
  this->direction = direction.direction;
}

Direction::~Direction() {

}

void Direction::SetDescription(std::string description) {
  this->description = description;
}

std::string Direction::GetDescription() const {
  return description;
}

void Direction::SetDirection(char direction) {
  this->direction = direction;
}

char Direction::GetDirection() const {
  return direction;
}

Item::Item() {
  name = "An nondescript item";
  description = "Unremarkable in every way";
  health = 0;
  strength = 0;
  secret = 0;
}

Item::Item(std::string name, std::string description, int health, int strength, int secret) {
  this->name = name;
  this->description = description;
  this->health = health;
  this->strength = strength;
  this->secret = secret;
}

Item::Item(const Item &orig) {
  this->name = orig.name;
  this->description = orig.description;
  this->health = orig.health;
  this->strength = orig.strength;
  this->secret = orig.secret;
}

Item::~Item() {

}

void Item::SetSecret(int secret) {
  this->secret = secret;
}

int Item::GetSecret() const {
  return secret;
}

void Item::SetStrength(int strength) {
  this->strength = strength;
}

int Item::GetStrength() const {
  return strength;
}

void Item::SetHealth(int health) {
  this->health = health;
}

int Item::GetHealth() const {
  return health;
}

void Item::SetDescription(std::string description) {
  this->description = description;
}

std::string Item::GetDescription() const {
  return description;
}

void Item::SetName(std::string name) {
  this->name = name;
}

std::string Item::GetName() const {
  return name;
}

Room::Room() {
  name = "Empty Room";
  description = "Just an empty, uninitialized room.";
}

Room::Room(std::string name, std::string description, int id) {
  this->name = name;
  this->description = description;
  this->id = id;
}

void Room::SetDescription(std::string description) {
  this->description = description;
}

std::string Room::GetDescription() const {
  return description;
}

void Room::SetName(std::string name) {
  this->name = name;
}

std::string Room::GetName() const {
  return name;
}

Room* Room::GetDirection(char direction) {
  if (links.find(direction) == links.end()) {
    error_reason = "Hmm, there doesn't appear to be a path in that direction.";
    return NULL;
  } else
    return links[direction];
}

void Room::SetId(int id) {
  this->id = id;
}

int Room::GetId() const {
  return id;
}

bool Room::CheckMove(char direction) {
  error_reason = "";
  if (links.find(direction) == links.end()) {
    error_reason = "There is no path in that direction";
    return false;
  }
  return true;
}

void Room::LinkRooms(Direction &dir, Room &room) {
  char direction = dir.GetDirection();
  std::map<char, Direction*>::iterator it = directions.find(direction);
  if (it == directions.end())
    directions[direction] = new Direction(dir);

  links[direction] = &room;
}

/* Briefly described in the header file. */
std::vector<std::string> Room::ListDirections() {
  std::vector<std::string> list_directions;
  for (std::map<char, Direction*>::iterator it = directions.begin(); it != directions.end(); ++it) {
    std::stringstream temp;
    temp << "(" << it->first << ") " << it->second->GetDescription();
    list_directions.push_back(temp.str());
  }
  return list_directions;
}

void Room::AddItem(Item &item) {
  Item *room_item = new Item();

  room_item->SetName(item.GetName());
  room_item->SetDescription(item.GetDescription());
  room_item->SetHealth(item.GetHealth());
  room_item->SetStrength(item.GetStrength());
  room_item->SetSecret(item.GetSecret());

  items.push_back(room_item);
}

Item* Room::ViewItem(unsigned long id) {
  if (id < items.size()) {
    return items[id];
  } else {
    error_reason = "That item doesn't exist!";
    return NULL;
  }
}

Item* Room::RetrieveItem(unsigned long id) {
  if (id < items.size()) {
    items.erase(items.begin() + id);
    return items[id];
  } else {
    error_reason = "That item doesn't exist!";
    return NULL;
  }
}

std::vector<std::string> Room::ListItems() {
  std::vector<std::string> list_items;
  unsigned long max_items = items.size();
  for (unsigned long index = 0; index < max_items; ++index) {
    std::stringstream temp;
    temp << "(" << index << ") " << items[index]->GetName() << " - ";
    temp << items[index]->GetDescription();
    list_items.push_back(temp.str());
  }
  return list_items;
}

Room::~Room() {
  for (std::map<char, Direction*>::iterator it = directions.begin(); it != directions.end(); ++it) {
    delete it->second;
  }
}

Player::Player() {
  health = 1000;
  strength = 10;
}

Player::~Player() {

}

std::string Room::GetErrorReason() const {
  return error_reason;
}

void Player::SetStrength(int strength) {
  this->strength = strength;
}

int Player::GetStrength() const {
  return strength;
}

void Player::SetHealth(int health) {
  this->health = health;
}

int Player::GetHealth() const {
  return health;
}

void Player::AddItem(Item &item) {
  items.push_back(new Item(item));
  strength += item.GetStrength();
}

bool Player::ConsumeItem(unsigned long id) {
  error_reason = "";
  if (id >= items.size()) {
    error_reason = "I don't have that item!";
    return false;
  } else {
    if (items[id]->GetHealth() > 0) {
      health += items[id]->GetHealth();
      strength -= items[id]->GetStrength();
      delete items[id];
      items.erase(items.begin() + id);
      return true;
    } else {
      error_reason = "I fail to see how I can consume this item...";
      return false;
    }
  }
}

std::vector<std::string> Player::ListPlayerDetails() {
  std::vector<std::string> list_details;
  std::stringstream temp;

  temp << "Health: " << health << ". Strength: " << strength;
  list_details.push_back(temp.str());

  unsigned long max_items = items.size();
  for (unsigned long index = 0; index < max_items; ++index) {
    temp.str(std::string());
    temp.clear();
    temp << "(" << index << ") " << items[index]->GetName() << " - ";
    temp << items[index]->GetDescription();
    list_details.push_back(temp.str());
  }
  return list_details;
}

std::string Player::GetErrorReason() const {
  return error_reason;
}

/* 
 * This default constructor will set up a very basic dummy world.
 * Generally, you will want to use the World(const char*) constructor as it
 * will parse an XML file to generate the world data.
 */
World::World() {
  rooms[0] = new Room("Spawn Room", "Home, sweet, home!", 0);

  current_room = rooms[0];
  player = new Player();
}

/* 
 * Generate the world from an XML configuration file. See world.xml for an
 * example.
 */
World::World(const char* filename) {
  world_file = new rapidxml::file<>(filename);
  world_doc.parse<0>(world_file->data());

  rapidxml::xml_node<> *part_node;
  rapidxml::xml_node<> *base_node = world_doc.first_node("world");
  base_node = base_node->first_node("rooms");

  for (part_node = base_node->first_node("room"); part_node; part_node = part_node->next_sibling("room")) {
    AddRoom(part_node);
  }

  base_node = base_node->parent();
  base_node = base_node->first_node("links");
  for (part_node = base_node->first_node("link"); part_node; part_node = part_node->next_sibling("link")) {
    AddLink(part_node);
  }

  base_node = base_node->parent();
  base_node = base_node->first_node("items");
  for (part_node = base_node->first_node("item"); part_node; part_node = part_node->next_sibling("item")) {
    AddItem(part_node);
  }
  current_room = rooms[0];
  player = new Player();
}

/*
 * A private method used to add a room from an XML definition.
 */
void World::AddRoom(rapidxml::xml_node<> *room_node) {
  int id = atoi(room_node->first_attribute("id")->value());
  rooms[id] = new Room(room_node->first_attribute("name")->value(),
          room_node->first_attribute("desc")->value(),
          id);
}

/*
 * A private method used to add a link between rooms from an XML definition.
 */
void World::AddLink(rapidxml::xml_node<> *link_node) {
  int from_id, to_id;
  rapidxml::xml_attribute<> *desc = NULL;
  Direction direction;

  from_id = atoi(link_node->first_attribute("from")->value());
  to_id = atoi(link_node->first_attribute("to")->value());

  direction.SetDirection(link_node->first_attribute("dir")->value()[0]);

  desc = link_node->first_attribute("desc");
  if (desc != NULL) {
    direction.SetDescription(desc->value());
  }

  rooms[from_id]->LinkRooms(direction, *rooms[to_id]);
}

/*
 * A private method used to add an item into a room from an XML definition.
 */
void World::AddItem(rapidxml::xml_node<> *item_node) {
  int room_id = 0;
  rapidxml::xml_attribute<> *name = NULL, *desc = NULL, *health = NULL,
          *strength = NULL, *secret = NULL;
  Item item;

  room_id = atoi(item_node->first_attribute("room")->value());
  health = item_node->first_attribute("health");
  strength = item_node->first_attribute("strength");
  secret = item_node->first_attribute("secret");
  name = item_node->first_attribute("name");
  desc = item_node->first_attribute("desc");

  if (health != NULL) {
    item.SetHealth(atoi(health->value()));
  }
  if (strength != NULL) {
    item.SetStrength(atoi(strength->value()));
  }
  if (secret != NULL) {
    item.SetSecret(atoi(secret->value()));
  }

  if (name != NULL) {
    item.SetName(name->value());
  }
  if (desc != NULL) {
    item.SetDescription(desc->value());
  }

  rooms[room_id]->AddItem(item);
}

World::World(const World & orig) {
}

World::~World() {
  for (std::map<int, Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
    delete it->second;
  }
  delete player;
  delete world_file;
}

std::string World::GetRoomName() const {
  return current_room->GetName();
}

std::string World::GetRoomDescription() const {
  return current_room->GetDescription();
}

bool World::MoveRoom(char direction) {
  if (!current_room->CheckMove(direction)) {
    error_reason = current_room->GetErrorReason();
    return false;
  }
  error_reason = "";

  current_room = current_room->GetDirection(direction);
  return true;
}

bool World::PickUpItem(unsigned long id) {
  Item *item = current_room->RetrieveItem(id);

  if (item == NULL) {
    error_reason = "I don't see that item";
    return false;
  }
  player->AddItem(*item);

  delete item;

  return true;
}

bool World::ConsumeItem(unsigned long id) {
  if(!player->ConsumeItem(id)) {
    error_reason = player->GetErrorReason();
    return false;
  }
  return true;
}
/*
 * Move commands are inputted by the user in the form of:
 * m:<direction> where <direction> is the char to denote a direction. E.g.:
 *    m:n
 */
bool World::DoCommand(std::string command) {
  std::vector<std::string> elems = split(command, ';');

  if (command.length() == 1) {
    return MoveRoom(command[0]);
  }
  if (elems[0].compare("m") == 0) {
    return MoveRoom(elems[1][0]);
  }
  if (elems[0].compare("p") == 0) {
    return PickUpItem(atoi(elems[1].c_str()));
  }
  if (elems[0].compare("c") == 0) {
    return ConsumeItem(atoi(elems[1].c_str()));
  }

  error_reason = "I don't understand";
  return false;
}

/*
 * Used by DoCommand to separate out the command type from the command parameter
 */
std::vector<std::string> World::split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

std::string World::GetErrorReason() const {
  return error_reason;
}

std::vector<std::string> World::ListDirections() {
  return current_room->ListDirections();
}

std::vector<std::string> World::ListRoomItems() {
  return current_room->ListItems();
}

std::vector<std::string> World::ListPlayerDetails() {
  return player->ListPlayerDetails();
}
