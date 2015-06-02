module Q06 where
import Q05

isPalindrome :: (Eq a) => [a] -> Bool
isPalindrome x = x == myreverse x