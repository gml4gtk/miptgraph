#include "layout_iface.h"

NodeAux::NodeAux( GraphAux *graph_p, int _id):
    Node( static_cast<Graph *>( graph_p), _id)
{
		setReal (true);
}
//-----------------------------------------------------------------------------
NodePropertiesAux::NodePropertiesAux()
{
    // Default position is first in first layer
    rang_priv = 0;
    pos_priv = 1;

	width_priv = 0;
	height_priv = 0;
	is_real = true;
}
//-----------------------------------------------------------------------------
void NodeAux::debugPrint()
{
	Node::debugPrint();
	out (" rang: %i; pos: %i; real:%i; x:%d", rang_priv, pos_priv, (int)real(), x());
}
//-----------------------------------------------------------------------------

/**
 *  Write to xml
 */
void NodeAux::writeByXmlWriter( xmlTextWriterPtr writer)
{
	Node::writeByXmlWriter (writer);
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "x", "%d", x());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "y", "%d", y());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "width", "%d", width());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "height", "%d", height());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "real", "%d", (int)real());
}
//-----------------------------------------------------------------------------
/**
 *  Read from xml
 */
void NodeAux::readByXml (xmlNode * cur_node)
{
	Node::readByXml (cur_node);

	for (xmlAttr* props = cur_node->properties; props; props = props->next)
	{
		if ( xmlStrEqual( props->name, xmlCharStrdup("x")))
		{
			msetX( strtoul( ( const char *)( props->children->content), NULL, 0));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup("y")))
		{
			msetY( strtoul( ( const char *)( props->children->content), NULL, 0));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup("real")))
		{
			setReal ((bool)strtoul( ( const char *)( props->children->content), NULL, 0));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup("width")))
		{
			setWidth( strtoul( ( const char *)( props->children->content), NULL, 0));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup("height")))
		{
			setHeight( strtoul( ( const char *)( props->children->content), NULL, 0));
		}
	}
}
//-----------------------------------------------------------------------------
