namespace SuffixTreePrototype
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    // Still O(m^3) - skip count trick as in page 101
    class SuffixTree2
    {
        private SuffixTree2(string text)
        {
            // Avoid direct construction
            this.text = text;
        }

        public static SuffixTree2 Build(string text)
        {
            // Step 1: Build initial tree
            SuffixTree2 result = new SuffixTree2(text);
            result.rootNode = new SuffixTreeNode();
            string suffix = text[0] + "";
            result.rootNode.links.Add(suffix[0], new SuffixTreeLink { Start = 0, End = 1, child = new SuffixTreeNode() });

            // Step 2: Basic extension loop
            for (int phase = 2; phase <= text.Length; phase++)
            {
                for (int j = 1; j <= phase; j++)
                {
                    // Extension j is about adding the string x[j..phase] into the suffix tree
                    result.Extend(text, j, phase);
                }
            }
            return result;
        }

        private void Extend(string text, int j, int phase)
        {
            string searchText = text.Substring(j - 1, phase - j);
            int searchingTextLength = phase - j; // Length of string to insert - 1 = (phase - j + 1) - 1 = phase - j
            int remainingTextLength = searchingTextLength;
            int textCursor = j;
            SuffixTreeNode nodeCursor = this.rootNode;
            SuffixTreeLink followingLink = null;
            int linkCursor = -1;

            while (remainingTextLength > 0)
            {
                char currentCharacter = text[textCursor - 1];
                if (followingLink == null)
                {
                    followingLink = nodeCursor.links[currentCharacter];
                    nodeCursor = null;
                    linkCursor = 0;
                }
                else
                {
                    if (linkCursor == followingLink.Length())
                    {
                        linkCursor = -1;
                        nodeCursor = followingLink.child;
                        followingLink = null;
                    }
                    else
                    {
                        int move = Math.Min(followingLink.Length(), remainingTextLength);
                        textCursor += move;
                        remainingTextLength -= move;
                        linkCursor += move;
                    }
                }
            }

            char characterToExtend = text[phase - 1];
            if (followingLink != null)
            {
                if (linkCursor == followingLink.Length())
                {
                    if (followingLink.child.links.Count == 0)
                    {
                        // Rule 1 - we have reached a leaf - extend the leaf, done
                        followingLink.End++;
                    }
                    else
                    {
                        if (followingLink.child.links.ContainsKey(characterToExtend))
                        {
                            // Rule 3 - no op
                        }
                        else
                        {
                            // Rule 2
                            followingLink.child.links.Add(characterToExtend, new SuffixTreeLink { Start = phase - 1, End = phase, child = new SuffixTreeNode() });
                        }
                    }
                }
                else
                {
                    if (followingLink.EdgeLabel(text, linkCursor) == characterToExtend)
                    {
                        // Rule 3 - no op
                    }
                    else
                    {
                        // Rule 2 
                        int originalEnd = followingLink.End;
                        SuffixTreeNode originalChild = followingLink.child;

                        followingLink.child = new SuffixTreeNode();
                        followingLink.End = followingLink.Start + linkCursor;
                        followingLink.child.links.Add(text[followingLink.Start + linkCursor], new SuffixTreeLink { Start = followingLink.Start + linkCursor, End = originalEnd, child = originalChild });
                        followingLink.child.links.Add(characterToExtend, new SuffixTreeLink { Start = phase - 1, End = phase, child = new SuffixTreeNode() });
                    }
                }
            }
            else
            {
                if (nodeCursor.links.ContainsKey(characterToExtend))
                {
                    // Rule 3 - no op
                }
                else
                {
                    // Rule 2 again
                    nodeCursor.links.Add(characterToExtend, new SuffixTreeLink { Start = phase - 1, End = phase, child = new SuffixTreeNode() });
                }
            }
        }

        private class SuffixTreeLink
        {
            // 0 based [) index
            public int Start { get; set; }
            public int End { get; set; }
            public char EdgeLabel(string text, int index)
            {
                return text[this.Start + index];
            }

            public string EdgeLabel(string text)
            {
                return text.Substring(this.Start, this.Length());
            }

            public int Length()
            {
                return this.End - this.Start;
            }

            public SuffixTreeNode child;
        }

        private class SuffixTreeNode
        {
            public Dictionary<char, SuffixTreeLink> links = new Dictionary<char, SuffixTreeLink>();

            internal void BuildString(string text, StringBuilder sb, int indent)
            {
                foreach (var link in links)
                {
                    sb.Append(new string('-', indent));
                    sb.AppendLine(link.Value.EdgeLabel(text));
                    link.Value.child.BuildString(text, sb, indent + 2);
                }
            }
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            rootNode.BuildString(this.text, sb, 0);
            return sb.ToString();
        }

        private SuffixTreeNode rootNode;
        private string text;
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine(SuffixTree2.Build("Hello World"));
        }
    }
}
