namespace SuffixTreePrototype
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    // Naive O(m^3) algorithm
    class SuffixTree1
    {
        private SuffixTree1()
        {
            // Avoid direct construction
        }

        public static SuffixTree1 Build(string text)
        {
            // Step 1: Build initial tree
            SuffixTree1 result = new SuffixTree1();
            result.rootNode = new SuffixTreeNode();
            string suffix = text[0] + "";
            result.rootNode.links.Add(suffix[0], new SuffixTreeLink { EdgeLabel = suffix, child = new SuffixTreeNode() });

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
                    if (linkCursor == followingLink.EdgeLabel.Length)
                    {
                        linkCursor = -1;
                        nodeCursor = followingLink.child;
                        followingLink = null;
                    }
                    else
                    {
                        if (currentCharacter == followingLink.EdgeLabel[linkCursor])
                        {
                            textCursor++;
                            remainingTextLength--;
                            linkCursor++;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }

            char characterToExtend = text[phase - 1];
            if (followingLink != null)
            {
                if (linkCursor == followingLink.EdgeLabel.Length)
                {
                    if (followingLink.child.links.Count == 0)
                    {
                        // Rule 1 - we have reached a leaf - extend the leaf, done
                        followingLink.EdgeLabel += characterToExtend;
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
                            followingLink.child.links.Add(characterToExtend, new SuffixTreeLink { EdgeLabel = characterToExtend + "", child = new SuffixTreeNode() });
                        }
                    }
                }
                else
                {
                    if (followingLink.EdgeLabel[linkCursor] == characterToExtend)
                    {
                        // Rule 3 - no op
                    }
                    else
                    {
                        // Rule 2 
                        string followingLinkEdgeLabelPrefix = followingLink.EdgeLabel.Substring(0, linkCursor);
                        string followingLinkEdgeLabelSuffix = followingLink.EdgeLabel.Substring(linkCursor);
                        SuffixTreeNode originalChild = followingLink.child;
                        followingLink.child = new SuffixTreeNode();
                        followingLink.EdgeLabel = followingLinkEdgeLabelPrefix;
                        followingLink.child.links.Add(followingLinkEdgeLabelSuffix[0], new SuffixTreeLink { EdgeLabel = followingLinkEdgeLabelSuffix, child = originalChild });
                        followingLink.child.links.Add(characterToExtend, new SuffixTreeLink { EdgeLabel = characterToExtend + "", child = new SuffixTreeNode() });
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
                    nodeCursor.links.Add(characterToExtend, new SuffixTreeLink { EdgeLabel = characterToExtend + "", child = new SuffixTreeNode() });
                }
            }
        }

        private class SuffixTreeLink
        {
            public string EdgeLabel { get; set; }
            public SuffixTreeNode child;
        }

        private class SuffixTreeNode
        {
            public Dictionary<char, SuffixTreeLink> links = new Dictionary<char, SuffixTreeLink>();

            internal void BuildString(StringBuilder sb, int indent)
            {
                foreach (var link in links)
                {
                    sb.Append(new string('-', indent));
                    sb.AppendLine(link.Value.EdgeLabel);
                    link.Value.child.BuildString(sb, indent + 2);
                }
            }
        }

        private SuffixTreeNode rootNode;

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            rootNode.BuildString(sb, 0);
            return sb.ToString();
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine(SuffixTree1.Build("Hello World"));
        }
    }
}
