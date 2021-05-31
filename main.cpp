#include <iostream>
#include <functional>

#include <flecs.h>

using namespace std;

/* Component type */
struct Message {
  const char* text;
};

struct Nutrition {
  int calories;
};

struct Comfort {
  int value;
  int min;
  int max;
};

struct Hunger {
  int value;
  int min;
  int max;
  int loss_per_second;
};

struct Stats {
  Comfort comfort;
  Hunger hunger;
};

struct Hungry {};

struct Comfortable {
  int comfort_per_second;
};

void update_hunger(flecs::iter& it, Hunger* hunger) {
  for (auto& i : it) {
    hunger[i].value -= hunger[i].loss_per_second;
    cout << it.entity(i) << " hunger: " << hunger[i].value << endl;
    if (hunger[i].value < hunger[i].min) {
      it.entity(i).add<Hungry>();
    }
  }
}


flecs::query<Nutrition> food_query;

void update_hungry(flecs::iter& it, Hungry* hungry, Hunger* hunger) {
  for (auto& i : it) {
    //auto& hunger = hunger[i];
    flecs::entity entity_to_eat;
    Nutrition nutrition_to_get;
    food_query.each([&](flecs::entity e, Nutrition nutrition) {
      entity_to_eat = e;
      nutrition_to_get = nutrition;
      });
    it.entity(i).get_mut<Hunger>()->value += nutrition_to_get.calories;
    it.world().remove(entity_to_eat);
    cout << it.entity(i) << " just ate starfruit: " << it.entity(i);
  }
}


void update_comfort(flecs::iter& it, Comfortable comfortable) {
  //it.world().
}


int main(int argc, char* argv[]) {
  /* Create the world, pass arguments for overriding the number of threads,fps
   * or for starting the admin dashboard (see flecs.h for details). */
  flecs::world ecs(argc, argv);
  ecs.component<Hungry>();
  ecs.component<Nutrition>();


  for (int i = 0; i < 20; ++i) {
    ecs.entity().set<Nutrition>({ 10 });
  }
 
  food_query = ecs.query<Nutrition>();

  for (int i = 0; i < 20; ++i) {
    ecs.entity().set<Hunger>({ 100, 40, 100, 5 });
  }
  
  ecs.system<Hunger>("Hunger").iter(update_hunger);
  ecs.system<Hungry, Hunger>("Hungry").iter(update_hungry);
   // HungrySystem::update);


  ecs.set_target_fps(20);

  std::cout << "Application simple_system is running, press CTRL-C to exit..." << std::endl;

  /* Run systems */
  while (ecs.progress()) {
  
  }
}
