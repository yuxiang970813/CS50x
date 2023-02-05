-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See what happen at July 28, 2021, Humphrey Street
SELECT description
  FROM crime_scene_reports
 WHERE year = 2021 AND month = 7 AND day = 28
   AND street = "Humphrey Street";

-- View the interview at July 28, 2021
SELECT name, transcript
  FROM interviews
 WHERE year = 2021 AND month = 7 AND day = 28;

 -- Based on Ruth's transcript
 -- Find the cars left at 10.15am(+-10m), July 28, 2021
SELECT minute, license_plate
  FROM bakery_security_logs
 WHERE year = 2021 AND month = 7 AND day = 28
   AND hour = 10 AND minute >= 15 AND minute <= 25
   AND activity = "exit";

-- Based on Eugene's transcript
-- Find the account which withdraw at Leggett Street, July 28, 2021
SELECT account_number
  FROM atm_transactions
 WHERE year = 2021 AND month = 7 AND day = 28
   AND atm_location = "Leggett Street"
   AND transaction_type = "withdraw";

-- Based on Raymond's transcript
-- Find the phone calls duration less than 1 min at July 28, 2021
SELECT caller, receiver, duration
  FROM phone_calls
 WHERE year = 2021 AND month = 7 AND day = 28
   AND duration <= 60;

-- Find the earliest flight out of Fiftyville at July 29, 2021
  SELECT id, hour, minute
    FROM flights
   WHERE origin_airport_id IN
         (SELECT id
           FROM airports
          WHERE city = "Fiftyville")
     AND year = 2021 AND month = 7 AND day = 29
ORDER BY hour, minute
   LIMIT 1;

-- Find the passengers who take the earliest flight out of Fiftyville at July 29, 2021
SELECT passport_number
  FROM passengers
 WHERE flight_id IN
       (SELECT id
         FROM flights
        WHERE origin_airport_id IN
              (SELECT id
                FROM airports
               WHERE city = "Fiftyville")
          AND year = 2021 AND month = 7 AND day = 29
     ORDER BY hour, minute
        LIMIT 1);

-- Find the people name that match all condition
SELECT name
  FROM people
 WHERE phone_number IN
       (SELECT caller
         FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28
          AND duration <= 60)
   AND passport_number IN
       (SELECT passport_number
          FROM passengers
         WHERE flight_id IN
               (SELECT id
                  FROM flights
                 WHERE origin_airport_id IN
                       (SELECT id
                          FROM airports
                         WHERE city = "Fiftyville")
                   AND year = 2021 AND month = 7 AND day = 29
              ORDER BY hour, minute
                 LIMIT 1))
   AND license_plate IN
       (SELECT license_plate
          FROM bakery_security_logs
         WHERE year = 2021 AND month = 7 AND day = 28
           AND hour = 10 AND minute >= 15 AND minute <= 25
           AND activity = "exit")
   AND id IN
       (SELECT person_id
          FROM bank_accounts
         WHERE account_number IN
               (SELECT account_number
                 FROM atm_transactions
                WHERE year = 2021 AND month = 7 AND day = 28
                  AND atm_location = "Leggett Street"
                  AND transaction_type = "withdraw"));

-- Find the city the thief escaped to:
SELECT city
  FROM airports
 WHERE id IN
       (SELECT destination_airport_id
          FROM flights
         WHERE id IN
               (SELECT id
                  FROM flights
                 WHERE origin_airport_id IN
                       (SELECT id
                       FROM airports
                       WHERE city = "Fiftyville")
                   AND year = 2021 AND month = 7 AND day = 29
              ORDER BY hour, minute
                 LIMIT 1));

-- Find the the accomplice
-- Who answered the phone
SELECT name
  FROM people
 WHERE phone_number IN
       (SELECT receiver
          FROM phone_calls
         WHERE caller IN
               (SELECT phone_number
                  FROM people
                 WHERE name = "Bruce")
           AND year = 2021 AND month = 7 AND day = 28
           AND duration <= 60);