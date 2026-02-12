# Grocery Store Game Systems

A collection of gameplay systems designed for a grocery store simulation game built in Unreal Engine 4. These systems demonstrate data-driven design, performance optimization through instancing, hierarchical inventory management, and physics-based interactions.

---

## A quick tour of every class in this directory

__AGroceriesContainer__ - groceries container is a base for an object used for carrying multiple grocery actors inside of it.
At the time, my game only had 2: Shopping Cart and a Shopping Basket (both implemented as Blueprints)

__UGroceriesInventory__ - a component based inventory system. allows you to have "subinventories" (which are of the same class). The idea was that the player could have possessed mutliple grocery container actors (for example, moving a shopping cart while also carrying a basket) and I wanted a quick way to look up all the items they carry.

__InstancedGroceryActor__ and __SingleGroceryActor__ - both are visual representation of a grocery item (or a group of grocery items) but the single one allows me to simulate physics on it while the instanced actor class utilizes InstancedStaticMesh component to draw multiple instances of the same mesh more efficiently

__CheckOutQueue__ - a beefy system for building a line for the check out at the grocery store around a spline. It was designed to work with both the player controlled character and NPCs. the trigger box that indicates the last spot in the line moves dynamically as the line increases or decreases

__TwoInRow_ISM__ and __StoreShelfTool__ - part of the editor toolkit i was making for this project specifically. allows me to spawn instanced static meshes with randomly paced grocery items around it


## Overall

The system was very flexible and allowed me to easily add more content such as grocery types through a simple entry in a data table.
It also served as a good exercise in polymorphism and allowed me to get a better feel for which cases work well for creating child-parent hierarchies.
The queue actor is pretty questionale in terms of performance but at the time i was more focused on making systems that do their job - which this did

