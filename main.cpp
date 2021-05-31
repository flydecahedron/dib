#include <iostream>

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

struct Hungry {
  flecs::entity entity;
};

struct Comfortable {
  int comfort_per_second;
};

void update_hunger(flecs::iter& it, Hunger* hunger) {
  for (auto& i : it) {
    hunger[i].value - hunger[i].loss_per_second;
    if (hunger[i].value < hunger[i].min) {
      it.world().entity().add<Hungry>(it.entity(i));
    }
  }
}

void update_hungry(flecs::iter& it, Hungry* hungry) {
  auto food_query = it.world().query<Nutrition>();

  for (auto& i : it) {
    auto hunger = hungry[i].entity.get_mut<Hunger>();
    flecs::entity entity_to_eat;
    Nutrition nutrition_to_get;
    food_query.each([&](flecs::entity e, Nutrition nutrition) {
      entity_to_eat = e;
      nutrition_to_get = nutrition;
      });
    hunger->value += nutrition_to_get.calories;
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

  ecs.system<Message>()
    .each([](Message& messages) {
    std::cout << messages.text << std::endl;
      });

  ecs.entity().set<Message>({ "Hello Flecs!" });

  auto starfruit = ecs.type("Starfruit")
    .add<Nutrition>(10);
  for (int i = 0; i < 20; ++i) {
    ecs.entity().add(starfruit);
  }

  auto dwarf = ecs.entity() 
    .set<Hunger>({ 100, 100, 40, 5 });
  for (int i = 0; i < 20; ++i) {
    ecs.entity().add(dwarf);
  }
  
  ecs.system<Hunger>("Hunger").iter(update_hunger);
  ecs.system<Hungry>("Hungry").iter(update_hungry);


  ecs.set_target_fps(1);

  std::cout << "Application simple_system is running, press CTRL-C to exit..." << std::endl;

  /* Run systems */
  while (ecs.progress()) {}
}
