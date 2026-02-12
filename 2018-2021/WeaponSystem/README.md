# Weapon System (aka CroissantWeapon)

A module of my game that's responsible for defining weapons in the game and establishes the inventory/management unit for characters to carry these weapons

__WeaponManager__ - essentially an inventory component that you can hook up to your character. exposes a few parameters that allow you to modify the behavior of the weapon system in the game
__FWeaponInfo__ (defined in WeaponManager.h) - the structure that defines a weapon unit and allows me to populate every weapon i have in the game in a single data table asset.
__WeaponActor__ and its children classes __Melee__, __Throwable__ and __ShootingWeapon__ - the class that represents the weapon inside the game and handle the actual Fire logic
__WeaponStuff__ - an interface, which allows the object who implment it to react to being hit by a weapon
