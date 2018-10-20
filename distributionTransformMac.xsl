<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes"/>

  <xsl:template match="installer-gui-script">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
      <title>VDJartnet</title>
      <license file="COPYING"/>
      <readme file="about.html" mime-type="text/html" uti="public.html"/>
      <domains enable_localSystem="false" enable_anywhere="false" enable_currentUserHome="true"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
