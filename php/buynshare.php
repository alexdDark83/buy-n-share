<?php
require "Google/FlatBuffers/Constants.php";
require "Google/FlatBuffers/ByteBuffer.php";
require "Google/FlatBuffers/FlatbufferBuilder.php";
require "Google/FlatBuffers/Table.php";
require "Google/FlatBuffers/Struct.php";

require "bs/FridgeMealCards.php";
require "bs/FridgePurchases.php";
require "bs/FridgeUsers.php";
require "bs/MealCard.php";
require "bs/Purchase.php";
require "bs/User.php";
require "bs/Users.php";
require "bs/Fridge.php";
require "bs/FridgeUser.php";
require "bs/Geo.php";
require "bs/Meal.php";
require "bs/UserFridges.php";
require "bs/UserPurchases.php";

require "env.php";

// https://stackoverflow.com/questions/4356289/php-random-string-generator
function generateRandomString($length = 10) 
{
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) 
    {
		$randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}

function fb_user(
    $id,
    $cn,
    $key,
    $locale,
    $lat,
    $lon,
    $alt
)
{
    $builder = new Google\FlatBuffers\FlatbufferBuilder(0);
    $scn = $builder->createString($cn);
    $skey = $builder->createString($key);
    $slocale = $builder->createString($locale);
	bs\User::startUser($builder);
    bs\User::addId($builder, $id);
    bs\User::addCn($builder, $scn);
    bs\User::addKey($builder, $skey);
    bs\User::addLocale($builder, $slocale);
    bs\User::addGeo($builder, bs\Geo::createGeo($builder, $lat, $lon, $alt));
    $u = bs\User::EndUser($builder);
    $builder->Finish($u);
    return $builder->dataBuffer()->data();
}

function fb_users
(
    &$users
)
{
    $builder = new Google\FlatBuffers\FlatbufferBuilder(0);
	
	$ua = array();
	foreach ($users as $user)
	{
	    $scn = $builder->createString($user[1]);
	    $skey = $builder->createString($user[2]);
		$slocale = $builder->createString($user[3]);

		bs\User::startUser($builder);
		bs\User::addId($builder, $user[0]);
		bs\User::addCn($builder, $scn);

		bs\User::addKey($builder, $skey);
		bs\User::addLocale($builder, $slocale);
		bs\User::addGeo($builder, bs\Geo::createGeo($builder, $user[4], $user[5], $user[6]));

		$u = bs\User::EndUser($builder);
		array_push($ua, $u);
	}
	$uv = bs\Users::CreateUsersVector($builder, $ua);
	bs\Users::startUsers($builder);
	bs\Users::addUsers($builder, $uv);
	$uu = bs\Users::EndUsers($builder);
    $builder->Finish($uu);
    return $builder->dataBuffer()->data();
}

function print_user
(
    $b
)
{
	$buf = Google\FlatBuffers\ByteBuffer::wrap($b);
	$u = bs\User::getRootAsUser($buf);
	print $u->getId() . " " . $u->getCn() . " " . $u->getKey() . " " . $u->getLocale() . " " . $u->getGeo()->getLat() . " " . $u->getGeo()->getLon() . " " . $u->getGeo()->getAlt();
}

function add_user
(
    $cn,
    $key,
    $locale,
    $lat,
    $lon,
    $alt
)
{
    $conn = init();
    $q = pg_query_params($conn, 
		'INSERT INTO "user" (cn, key, locale, lat, lon, alt) VALUES ($1, $2, $3, $4, $5, $6) RETURNING id', 
		array($cn, $key, $locale, $lat, $lon, $alt)
    );
    $r = pg_fetch_row($q);
	if (!$q)
	{
	    done($conn);
		return false;
	}
    pg_free_result($q);
    done($conn);
    return $r[0];
}

function ls_user
(
	$locale
)
{
    $conn = init();
    $q = pg_query_params($conn, 
		"SELECT id, cn, '' as key, locale, lat, lon, alt FROM \"user\" WHERE (locale = $1) ORDER BY id", array($locale)
    );
	if (!$q)
	{
	    done($conn);
		return false;
	}
	$r = array();
	while($row = pg_fetch_row($q))
	{
		array_push($r, $row);
	}
    pg_free_result($q);
    done($conn);
    return $r;
}

?>
