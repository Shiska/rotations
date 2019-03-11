<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" version="5.0" encoding="UTF-8" omit-xml-declaration="yes"/>
<xsl:template match="/">
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <title><xsl:value-of select="doc/assembly/name"/></title>
    <style type="text/css">
        #body {
            display: flex;
            border-top: 1px solid black;
        }
        #index {
            min-height: 100vh;
        }
        .info, #index {
            padding: 0 1em 0 1em;
        }
        .info {
            border-left: 1px solid;
            width: 100%;
        }
        .hidden {
            display: none;
        }
        .inline {
            display: flex;
            align-items: center;
        }
        .param {
            font-weight: bold;
            font-style: italic;
        }
        .paraminfo {
            font-weight: bold;
            color: #336699;
        }
        body {
            display: flex;
            font-size: small;
            flex-direction: column;
            font-family: "Verdana", sans-serif;
        }
        h1,h2,h3,h4,th {
            color: #4e4887;
            margin: 0.5em 0.5em 0.5em 0em;
        }
        h1 { font-size: xx-large; }
        h2 { font-size: x-large; }
        h3 { font-size: large; }
        h4 { font-size: medium; }

        table {
            border: medium none;
            background-color: #ddeeff;
        }
        td, td>div>h4 {
            margin: 2px;
            padding: 2px;
            text-align: left;
            font-size: x-small;
            border: medium none;
        }
        <!-- CODE { FON-SIZE: small; } -->
        <!-- DL { MARGIN-LEFT: 4em; DISPLAY: compact } -->
        <!-- DT { FONT-WEIGHT: bold } -->
        <!-- A:link { COLOR: #4e4887 } -->
        <!-- A:visited { COLOR: #8080c8 } -->
        <!-- A:active { COLOR: #f16043 } -->
        <!-- A:hover { COLOR: #f16043 } -->
        <!-- P { MARGIN-BOTTOM: 0.5em; MARGIN-TOP: 0.5em; MARGIN-LEFT: 4em } -->
        <!-- P.noindent { MARGIN-LEFT: 0em } -->
        <!-- HR.para { HEIGHT: 0; BORDER: 0; COLOR: transparent; BACKGROUND-COLOR: transparent; MARGIN-TOP: 0.5em; MARGIN-BOTTOM: 0; } -->
        <!-- XMP { BACKGROUND-COLOR: #ddeeff; FONT-SIZE: x-small; MARGIN: 1em } -->
        <!-- PRE { BACKGROUND-COLOR: #ddeeff; FONT-SIZE: x-small; MARGIN: 1em } -->
        <!-- TH { BACKGROUND-COLOR: #336699; COLOR: #ddeeff; BORDER-BOTTOM: medium none; BORDER-LEFT: medium none; BORDER-RIGHT: medium none; BORDER-TOP: medium none; FONT-SIZE: x-small; MARGIN: 2px; PADDING-BOTTOM: 2px; PADDING-LEFT: 4px; PADDING-RIGHT: 4px; PADDING-TOP: 2px; TEXT-ALIGN: left } -->
        <!-- UL { MARGIN-TOP: 0.5em; } -->
        <!-- LI.referrer { DISPLAY: inline; PADDING-RIGHT: 8px } -->
        <!-- LI.dependency { DISPLAY: inline; PADDING-RIGHT: 8px } -->
        <!-- LI.seealso { DISPLAY: inline; PADDING-RIGHT: 8px } -->
        <!-- LI.attribute { DISPLAY: inline; PADDING-RIGHT: 8px } -->
        <!-- LI.post { DISPLAY: inline; PADDING-RIGHT: 8px } -->
        <!-- LI.author { DISPLAY: inline; PADDING-RIGHT: 8px } -->
        <!-- LI.fixes { DISPLAY: inline; PADDING-RIGHT: 8px } -->
        <!-- LI.see { DISPLAY: inline; PADDING-RIGHT: 8px } -->
        <!-- LI.changelog { DISPLAY: inline; } -->
        <!-- .symbol { PADDING-RIGHT: 8px } -->
        <!-- OL { MARGIN-TOP: 0.5em; } -->
        <!-- DIV.library { TEXT-ALIGN: center; BORDER-RIGHT: #4e4887 8px solid; BORDER-TOP: #4e4887 2px solid; COLOR: #4e4887; MARGIN-BOTTOM: 0.5em; MARGIN-TOP: 1em; } -->
        <!-- H1.library { TEXT-ALIGN: center; COLOR: #4e4887; MARGIN-TOP: 0.3em; } -->
        <!-- H2.library { TEXT-ALIGN: center; BORDER: none; } -->
        <!-- PRE { BACKGROUND-COLOR: #ddeeff; FONT-SIZE: small; MARGIN: 1em } -->
    </style>
    <script>
        var displayed = []

        function getSection(element) {
            while(element.tagName != "SECTION") {
                element = element.parentElement;
            }
            return element;
        }

        function display(event, id) {
            var dest = document.getElementById(id);

            if(dest) {
                dest = getSection(dest);

                var destidx = displayed.indexOf(dest);
                var src = getSection(event.target);
                var srcidx = displayed.indexOf(src) + 1;

                if(destidx == -1) { // element isn't visible
                    // get position of source section and remove all opened views after it
                    while(displayed.length > srcidx) {
                        displayed.pop().className = "hidden";
                    }
                    dest.className = "info";
                    // move new element behind source
                    src.parentNode.insertBefore(dest, src.nextSibling);
                    // add it to the array
                    displayed.push(dest);
                } else if(destidx > srcidx) { // element is visible but could be further away, so move it down
                    while(displayed.length > srcidx) {
                        displayed.pop().className = "hidden";
                    }
                    dest.className = "info";
                    // no need to move it because it is already behind source
                    displayed.push(dest);
                }
            } else { // id not found
                
            }
        }
    </script>
</head>
<body>
    <section>
        <h1><xsl:value-of select="doc/assembly/name"/></h1>
        <xsl:apply-templates select="doc/general/summary"/>
        <xsl:apply-templates select="doc/general/*[position() > 1]"/>
    </section>
    <section id="body">
        <xsl:variable name="members" select="doc/members/member[not(@name='F:__file' or @name='F:__date' or @name='F:__time')]"/>
        <section id="index">
            <xsl:call-template name="index">
                <xsl:with-param name="name" select="'enumeration'"/>
                <xsl:with-param name="export" select="$members[starts-with(@name,'T:')][export]"/>
            </xsl:call-template>
            <xsl:call-template name="index">
                <xsl:with-param name="name" select="'constant'"/>
                <xsl:with-param name="export" select="$members[starts-with(@name,'C:')][export]"/>
            </xsl:call-template>
            <xsl:call-template name="index">
                <xsl:with-param name="name" select="'variable'"/>
                <xsl:with-param name="export" select="$members[starts-with(@name,'F:')][export]"/>
            </xsl:call-template>
            <xsl:call-template name="index">
                <xsl:with-param name="name" select="'function'"/>
                <xsl:with-param name="export" select="$members[starts-with(@name,'M:')][export]"/>
            </xsl:call-template>
        </section>
        <xsl:apply-templates select="$members"/>
    </section>
</body>
</html>
</xsl:template>

<xsl:template match="general">
    <xsl:apply-templates/>
</xsl:template>

<xsl:template match="section"><h2 class="general"><xsl:apply-templates/></h2></xsl:template>
<xsl:template match="subsection"><h3 class="general"><xsl:apply-templates/></h3></xsl:template>

<xsl:template name="index">
    <xsl:param name="name"/>
    <xsl:param name="export"/>
    <xsl:if test="count($export) != 0">
        <xsl:variable name="Name" select="concat(translate(substring($name,1,1),'ecvf','ECVF'),substring($name,2))"/>
        <h2>
            <xsl:attribute name="class"><xsl:value-of select="$name"/></xsl:attribute>
            <xsl:value-of select="$Name"/>s
        </h2>
        <ul>
            <xsl:for-each select="$export">
                <xsl:variable name="sub" select="substring(@name,3)"/>
                <li>
                    <xsl:call-template name="ref">
                        <xsl:with-param name="id" select="$sub"/>
                    </xsl:call-template>
                </li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template match="member">
    <xsl:variable name="sub" select="substring(@name,3)"/>

    <section>
        <xsl:attribute name="class">hidden</xsl:attribute>
        <xsl:attribute name="id"><xsl:value-of select="$sub"/></xsl:attribute>
        <a>
            <xsl:attribute name="name"><xsl:value-of select="$sub"/></xsl:attribute>
            <h2><xsl:value-of select="$sub"/></h2>
        </a>
        <xsl:apply-templates select="summary"/>
        <xsl:call-template name="value"/>
        <xsl:call-template name="syntax"/>
        <xsl:call-template name="param"/>
        <xsl:apply-templates select="tagname"/>
        <xsl:apply-templates select="size"/>
        <xsl:call-template name="returns"/>
        <xsl:call-template name="remarks"/>
        <xsl:call-template name="enum"/>
        <xsl:call-template name="example"/>
        <xsl:call-template name="referrer"/>
        <xsl:call-template name="dependency"/>
        <xsl:call-template name="attribute"/>
        <xsl:apply-templates select="automaton"/>
        <xsl:call-template name="transition"/>
        <xsl:apply-templates select="stacksize"/>
        <xsl:call-template name="seealso"/>
    </section>
</xsl:template>

<xsl:template match="summary">
    <p><xsl:apply-templates/></p>
</xsl:template>

<xsl:template name="value">
    <xsl:if test="@value">
        <div class="inline">
            <h4>Value</h4>
            <xsl:value-of select="@value"/>
        </div>
    </xsl:if>
</xsl:template>

<xsl:template name="syntax">
    <xsl:if test="@syntax">
        <h4>Syntax</h4>
        <code><xsl:value-of select="@syntax"/></code>
    </xsl:if>
</xsl:template>

<xsl:template name="param">
    <xsl:if test="param">
        <p>
            <table>
                <xsl:for-each select="param">
                    <tr>
                        <td class="param"><xsl:value-of select="@name"/></td>
                        <td><xsl:apply-templates/></td>
                    </tr>
                </xsl:for-each>
            </table>
        </p>
    </xsl:if>
</xsl:template>

<xsl:template match="paraminfo">
    <span class="paraminfo">&lt;<xsl:apply-templates/>&gt;</span>
</xsl:template>

<xsl:template match="tagname">
    <div class="inline">
        <h4>Tag</h4>        
        <xsl:call-template name="ref">
            <xsl:with-param name="id" select="@value"/>
        </xsl:call-template>
    </div>
</xsl:template>

<xsl:template match="size">
    <div class="inline">
        <h4>Size</h4>
        <xsl:value-of select="@value"/>
    </div>
</xsl:template>

<xsl:template name="returns">
    <xsl:if test=".//returns">
        <h4>Returns</h4>
        <xsl:for-each select=".//returns">
            <p><xsl:apply-templates/></p>
        </xsl:for-each>
    </xsl:if>
</xsl:template>

<xsl:template name="remarks">
    <xsl:if test=".//remarks">
        <h4>Remarks</h4>
        <xsl:for-each select=".//remarks">
            <p><xsl:apply-templates/></p>
        </xsl:for-each>
    </xsl:if>
</xsl:template>

<xsl:template name="enum">
    <xsl:if test="member">
        <h4>Members</h4>
        <p>
            <table>
                <xsl:for-each select="member">
                    <xsl:variable name="sub" select="substring(@name,3)"/>
                    <tr>
                        <xsl:attribute name="id"><xsl:value-of select="$sub"/></xsl:attribute>
                        <td class="param">
                            <a>
                                <xsl:attribute name="name"><xsl:value-of select="$sub"/></xsl:attribute>
                                <xsl:value-of select="$sub"/>
                            </a>
                        </td>
                        <td><xsl:call-template name="value"/></td>

                        <xsl:if test="*">
                            <td><xsl:apply-templates/></td>
                        </xsl:if>
                    </tr>
                </xsl:for-each>
            </table>
        </p>
    </xsl:if>
</xsl:template>

<xsl:template name="example">
    <xsl:if test=".//example">
        <h4>Example</h4>
        <xsl:for-each select=".//example">
            <p><xsl:apply-templates/></p>
        </xsl:for-each>
    </xsl:if>
</xsl:template>

<xsl:template name="referrer">
    <xsl:if test="referrer">
        <h4>Used by</h4>
        <ul>
            <xsl:for-each select="referrer">
                <li class="referrer">
                    <xsl:call-template name="ref">
                        <xsl:with-param name="id" select="@name"/>
                    </xsl:call-template>
                </li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template name="dependency">
    <xsl:if test="dependency">
        <h4>Depends on</h4>
        <ul>
            <xsl:for-each select="dependency">
                <li class="dependency">
                    <xsl:call-template name="ref">
                        <xsl:with-param name="id" select="@name"/>
                    </xsl:call-template>
                </li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template name="attribute">
    <xsl:if test="attribute">
        <h4>Attributes</h4>
        <ul>
            <xsl:for-each select="attribute">
                <li class="attribute"><xsl:value-of select="@name"/></li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template match="automaton">
    <h4>Automaton</h4>
    <p><xsl:value-of select="@name"/></p>
</xsl:template>

<xsl:template name="transition">
    <xsl:if test="transition">
        <h4>Transition table</h4>
        <p>
            <table>
                <tr><th>Source</th><th>Target</th><th>Condition</th></tr>
                <xsl:for-each select="transition">
                    <tr>
                        <td class="transition"><xsl:value-of select="@source"/></td>
                        <td class="transition"><xsl:value-of select="@target"/></td>
                        <td><xsl:value-of select="@condition"/></td>
                    </tr>
                </xsl:for-each>
            </table>
        </p>
    </xsl:if>
</xsl:template>

<xsl:template match="stacksize">
    <h4>Estimated stack usage</h4>
    <p><xsl:value-of select="@value"/> cell<xsl:if test="@value > 1">s</xsl:if></p>
</xsl:template>

<xsl:template name="seealso">
    <xsl:if test=".//seealso">
        <h4>See Also</h4>
        <ul>
            <xsl:for-each select=".//seealso">
                <li>
                    <xsl:call-template name="ref">
                        <xsl:with-param name="id" select="@name"/>
                    </xsl:call-template>
                </li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template match="code">
    <pre><xsl:apply-templates/></pre>
</xsl:template>

<xsl:template match="paramref">
    <i class="param"><xsl:value-of select="@name"/></i>
</xsl:template>

<!-- pawn-lang html tags -->
<xsl:template match="c"><code><xsl:apply-templates/></code></xsl:template>
<xsl:template match="em"><em><xsl:apply-templates/></em></xsl:template>
<xsl:template match="p"><hr class="para"/><xsl:apply-templates/></xsl:template>
<xsl:template match="para"><hr class="para"/><xsl:apply-templates/></xsl:template>
<xsl:template match="ul"><ul><xsl:apply-templates/></ul></xsl:template>
<xsl:template match="ol"><ol><xsl:apply-templates/></ol></xsl:template>
<xsl:template match="li"><li><xsl:apply-templates/></li></xsl:template>
<!-- additional html tags -->
<xsl:template match="br"><br /></xsl:template>
<xsl:template match="a">
    <xsl:choose>
        <xsl:when test="node()">
            <a>
                <xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute>
                <xsl:attribute name="target"><xsl:value-of select="@target"/></xsl:attribute>
                <xsl:apply-templates/>
            </a>
        </xsl:when>
        <xsl:otherwise>
            <a>
                <xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute>
                <xsl:attribute name="target"><xsl:value-of select="@target"/></xsl:attribute>
                <xsl:value-of select="@href"/>
            </a>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template match="ref">
    <xsl:call-template name="ref">
        <xsl:with-param name="id" select="@name"/>
    </xsl:call-template>
</xsl:template>

<xsl:template name="ref">
    <xsl:param name="id"/>
    <xsl:choose>
        <xsl:when test="//member[concat(':', $id) = substring(@name, 2)]">
            <a>
                <xsl:attribute name="href">#<xsl:value-of select="$id"/></xsl:attribute>
                <xsl:attribute name="onclick">display(event, '<xsl:value-of select="$id"/>')</xsl:attribute>
                <xsl:value-of select="$id"/>
            </a>
        </xsl:when>
        <xsl:otherwise>
            <xsl:value-of select="$id"/>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

</xsl:stylesheet>

