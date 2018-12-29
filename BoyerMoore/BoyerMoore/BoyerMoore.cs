using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace BoyerMoore
{
    public static class BoyerMoore
    {
        public static IEnumerable<int> IndexOf(string text, string pattern)
        {
            if (pattern.Equals(""))
            {
                yield return 0;
                yield break;
            }

            // For an initial read of the algorithm, it is advisable to read this later, just assume you have a magic shift table computed
            int[] goodSuffixShifts = BuildGoodSuffixShifts(pattern);

            Dictionary<char, int[]> badCharacterShifts = BuildBadCharacterShifts(pattern);

            int i = 0;
            int goodSuffixCheck = -1;
            int goodSuffixSkip = 0;
            while (i + pattern.Length <= text.Length)
            {
                // Consider the pattern is placed so that its first character it under the ith character of the text
                // We started the matching from the right of the pattern
                int patternIndex = pattern.Length - 1;
                int textIndex = i + patternIndex;

                while (patternIndex >= 0 && text[textIndex] == pattern[patternIndex])
                {
                    textIndex--;
                    patternIndex--;

                    // Here is an optimization for the good suffix case
                    // If the shift is caused by a good suffix, we first count down the 
                    // number of characters shifted, as we knew nothing about the text for 
                    // those shifts
                    if (goodSuffixCheck > 0)
                    {
                        goodSuffixCheck--;
                    }

                    // Once we reach the point where we hit the known match
                    if (goodSuffixCheck == 0)
                    {
                        // Then we just skip all the known matches 
                        goodSuffixCheck = -1;
                        textIndex -= goodSuffixSkip;
                        patternIndex -= goodSuffixSkip;
                    }
                }

                // If the loop ended with patternIndex == -1
                if (patternIndex == -1)
                {
                    // The whole pattern matched
                    yield return i;
                }

                // Figure out how many characters we can shift the pattern
                int matched = pattern.Length - patternIndex - 1;
                Debug.Assert(matched <= pattern.Length);

                int badCharacterShift = 0;
                if (patternIndex != -1)
                {
                    // In case we have a mismatching character
                    char badCharacter = text[textIndex];
                    if (badCharacterShifts.ContainsKey(badCharacter))
                    {
                        // In case the bad character does exist in the pattern
                        // We find the right most occurence of it and align them
                        badCharacterShift = badCharacterShifts[badCharacter][patternIndex];
                    }
                    else
                    {
                        // Otherwise, the bad character is not seen, we can shift the pattern 
                        // so that it is after the bad character
                        badCharacterShift = patternIndex + 1;
                    }
                }

                int goodSuffixShift = 0;

                if (goodSuffixShifts[matched] == 0)
                {
                    // Ideally we would like to start compare the with the pattern shifted by the pattern length, for example, we can have:
                    //
                    // text                 : abceabcd
                    // pattern before shift : abcd
                    // pattern after shift  :     abcd
                    //
                    goodSuffixShift = pattern.Length;
                }
                else
                {
                    // But it isn't always possible, in case a suffix of the pattern repeats within itself, for example, we can have:
                    //
                    // text                 : aaaaaaapqbbbbbbbbbbbb
                    // pattern before shift : pqaxpqxpq
                    // pattern after shift  :        pqaxpqxpq
                    //
                    // In the case above, we have determined that we have 2 matching characters, how do we determine that we should shift by 7 characters?
                    // The key idea is that we know not just we have 2 matching character, we also know the third character doesn't match, 
                    // this leads us to the notion of maximal suffix.
                    //
                    // A substring of the pattern is a maximal suffix if it matches a proper suffix of the pattern and either it can't extend the the left or
                    // if it extended to the left, it is no longer matches a suffix pattern.
                    //
                    // In the example above, we have two maximal suffixes:
                    // **
                    //    ***
                    // pqaxpqxpq
                    // 
                    // After the first match, we know 2 characters matches, therefore we would shift the pattern trying to align the length 2 maximal suffix
                    // to the matched characters, we get the 7 characters shift as expected.
                    //
                    // The key question would be, why is it correct?
                    // If we had a length 1 maximal suffix, aligning that one is not going to work, as we will fail at the second character.
                    // If we had a length 3 maximal suffix, aligning that one is not going to work either, as we will fail at the third character, 
                    // as we knew the third character of the text doesn't match with the pattern, but the length 3 maximal suffix does
                    //
                    // There is just one last twist, if we had more than one length 2 maximal suffix, we need to align it to the rightmost one to make sure
                    // we don't skip potential hits
                    //
                    // Now go read the ComputeShiftTable() to see how the maximal suffixes are found and the shift table constructed
                    //
                    goodSuffixShift = goodSuffixShifts[matched];
                }

                if (badCharacterShift > goodSuffixShift)
                {
                    // In case we have a bad character shift, we know nothing about what will match
                    // after the shift (except, of course, the only character we are aligning to)
                    // so we switch the goodSuffixCheck off
                    goodSuffixCheck = -1;
                    goodSuffixSkip = 0;
                    i += badCharacterShift;
                }
                else
                {
                    // In case we have a good suffix shift, we know the shifted characters are unknown
                    // so they must be checked
                    goodSuffixCheck = goodSuffixShift;

                    // After going through the shifted characters, we know we reach a point where the string 
                    // aligns
                    if (matched == pattern.Length)
                    {
                        // In case the whole pattern is matched, we know the full prefix matches
                        goodSuffixSkip = matched - goodSuffixShift;
                    }
                    else
                    {
                        // Otherwise, we know the number of matched character in the suffix
                        goodSuffixSkip = matched;
                    }
                    i += goodSuffixShift;
                }
            }
        }

        private static Dictionary<char, int[]> BuildBadCharacterShifts(string pattern)
        {
            Dictionary<char, int[]> badCharacterShifts = new Dictionary<char, int[]>();
            for (int i = 0; i < pattern.Length; i++)
            {
                char c = pattern[i];
                int[] shift;
                if (!badCharacterShifts.TryGetValue(c, out shift))
                {
                    // A new array is by default zero filled
                    shift = new int[pattern.Length];
                    badCharacterShifts.Add(c, shift);
                }

                // Therefore, we have a 1 in the array at the corresponding index if the 
                // character appears, and 0 otherwise
                shift[i] = 1;
            }

            foreach (int[] shift in badCharacterShifts.Values)
            {
                // A placeholder value to indicate the array is never shifted
                int lastShift = -1;
                for (int i = 0; i < shift.Length; i++)
                {
                    if (shift[i] == 0)
                    {
                        // At this point we are at a mismatch
                        if (lastShift == -1)
                        {
                            // There is no occurrence of the character before this position
                            // Therefore we can safely shift the pattern past the character
                            shift[i] = i + 1;
                        }
                        else
                        {
                            // Last time we shifted lastShift characters and then it aligns
                            // Therefore we can shift one more character now to align
                            shift[i] = ++lastShift;
                        }
                    }
                    else
                    {
                        // Here we have as hit, the driver should not access this value
                        shift[i] = -1;
                        // And we need to shift no character to achieve alignment now
                        lastShift = 0;
                    }
                }
            }

            return badCharacterShifts;
        }

        private static int[] BuildGoodSuffixShifts(string s)
        {
            int length = s.Length;
            int[] maximalSuffixLengths = new int[length - 1];
            int left = -1;
            int right = 0;
            for (int i = s.Length - 2; i >= 0; i--)
            {
                int currentLeft = i;
                int currentRight = i + 1;
                int prefixLeft = length - 1;
                if (left != -1)
                {
                    // Here we have a maximal suffix, as always, we have:
                    // s[left, right) = s[left + length - right, length)
                    Debug.Assert(IsMaximalSuffix(s, left, right));

                    if (left <= i && i < right)
                    {
                        // Now we know s[i] lies inside the leftmost maximal suffix
                        // In particular, s[left, i + 1) = s[left + length - right, i + length - right + 1)

                        // So we are interested to see the maximal suffix starting from i + length - right
                        int knownRight = i + length - right + 1;

                        //   knownRight - 1 - i 
                        // = i + length - right + 1 - 1 - i
                        // = length - right > 0
                        // Therefore we know knownRight - 1 > i - we are always accessing the array that must have been already populated
                        Debug.Assert(knownRight - 1 > i);
                        int knownLength = maximalSuffixLengths[knownRight - 1];

                        int knownLeft = knownRight - knownLength;
                        Debug.Assert(knownLeft >= 0);

                        // In terms of the variables, we have
                        // s[knownLeft, knownRight) = s[knownLeft + length - knownRight, length)
                        Debug.Assert(IsSuffix(s, knownLeft, knownRight));

                        // We wish to use the relation s[left, i + 1) = s[left + length - right, i + length - right + 1)
                        // So we need to make sure s[knownLeft, knownRight) is substring of s[left + length - right, i + length - right + 1)
                        if (knownLeft < left + length - right)
                        {
                            knownLeft = left + length - right;
                        }
                        Debug.Assert(left + length - right <= knownLeft && knownLeft < length);
                        Debug.Assert(left + length - right < knownRight && knownRight <= length);

                        // Now we can shift them back, and this now we have this
                        // s[knownLeft, knownRight = i + 1) = s[knownLeft + length - knownRight, length)
                        knownLeft = knownLeft + right - length;
                        knownRight = knownRight + right - length;
                        Debug.Assert(knownRight == i + 1);
                        Debug.Assert(IsSuffix(s, knownLeft, knownRight));

                        currentLeft = knownLeft - 1;
                        prefixLeft = knownLeft + length - knownRight - 1;
                    }
                }

                // Now, we extend the maximal suffix until we cannot

                // Note that in this loop, we are either exploring characters already discovered in a known maximal suffix
                // in which the loop should terminate right away because the maximal suffix terminated inside, or we are 
                // discovering new characters. Therefore the total time spent on this loop should be proportional to the length of s
                while (currentLeft >= 0 && s[currentLeft] == s[prefixLeft])
                {
                    currentLeft--;
                    prefixLeft--;
                }

                // We moved too much, adjust back
                currentLeft++;
                prefixLeft++;

                // Now we have found the maximal suffix
                Debug.Assert(IsMaximalSuffix(s, currentLeft, currentRight));

                // Book keeping for the left most maximal suffix
                if (left == -1 || currentLeft < left)
                {
                    left = currentLeft;
                    right = currentRight;
                }

                // And save the lengths
                maximalSuffixLengths[i] = currentRight - currentLeft;
            }

            // Make sure we have got it right in debug mode
            for (int i = 0; i < s.Length - 1; i++)
            {
                Debug.Assert(IsMaximalSuffix(s, i + 1 - maximalSuffixLengths[i], i + 1));
            }

            // Now we compute the shift table, the number of character matched could range from 0 to length
            int[] shifts = new int[length + 1];

            // Starting from the back
            for (int i = length - 2; i >= 0; i--)
            {
                int maximalSuffixLength = maximalSuffixLengths[i];

                // Note that i + shift = length - 1, this is designed so that the ith character aligns with the last character.
                int shift = length - 1 - i;

                if (shifts[maximalSuffixLength] == 0)
                {
                    shifts[maximalSuffixLength] = shift;
                }
                if (shifts[length] == 0 && maximalSuffixLength == (i + 1))
                {
                    //
                    // In case the full pattern is matched, we will never have a maximal suffix with length n (it has to be proper)
                    // Therefore the rule above cannot handle that special case, and must be analyzed differently
                    //
                    // Suppose a full pattern is matched:
                    // text    : abcabcxxxxxxx
                    // pattern : abcabc
                    // 
                    // In this case we know we should shift by 3, because the prefix matches a suffix. We can see that for any 
                    // shift less than the pattern length, that has to be the case.
                    // 
                    // We can detect the prefix matches a suffix case by checking the maximal suffix starts at 0. Note that
                    // when a maximal suffix starts at 0 and ends at i, it has length (i + 1), that is what the condition is checking
                    // 
                    // Again, if we have multiple ways such that prefix matches suffix, we pick the one with least shift to make sure 
                    // we capture all occurences, this could happen in this case:
                    //
                    // text    : aaxaaxaa
                    // pattern : aaxaa
                    //
                    // In this case we can only shift by 3, not 4.
                    //
                    shifts[length] = shift;
                }
            }

            return shifts;
        }

        private static string SubstringLeftRight(this string s, int left, int right)
        {
            if (left == s.Length)
            {
                Debug.Assert(right == s.Length);
                return "";
            }
            else
            {
                Debug.Assert(left >= 0);
                Debug.Assert(right >= 0);
                Debug.Assert(right <= s.Length);
                Debug.Assert(right >= left);
                return s.Substring(left, right - left);
            }
        }

        private static bool IsSuffix(string s, int left, int right)
        {
            return s.SubstringLeftRight(left, right) == s.SubstringLeftRight(left + s.Length - right, s.Length);
        }

        private static bool IsMaximalSuffix(string s, int left, int right)
        {
            if (IsSuffix(s, left, right))
            {
                if (left > 0)
                {
                    return !IsSuffix(s, left - 1, right);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }
    }
}
