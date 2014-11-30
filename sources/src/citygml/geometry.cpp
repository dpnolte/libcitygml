#include "citygml/geometry.h"

#include <citygml/polygon.h>
#include <citygml/appearancemanager.h>
#include <citygml/appearance.h>

namespace citygml {

    Geometry::Geometry(const std::string& id, Geometry::GeometryType type, unsigned int lod) : AppearanceTarget( id ), m_finished(false), m_type( type ), m_lod( lod )
    {

    }

    unsigned int Geometry::getLOD() const
    {
        return m_lod;
    }

    unsigned int Geometry::getPolygonsCount() const
    {
        return m_polygons.size();
    }

    Polygon& Geometry::getPolygon(unsigned int i)
    {
        return *m_polygons.at(i);
    }

    const Polygon& Geometry::getPolygon(unsigned int i) const
    {
        return *m_polygons.at(i);
    }

    unsigned int Geometry::getGeometriesCount() const
    {
        return m_childGeometries.size();
    }

    const Geometry& Geometry::getGeometry(unsigned int i) const
    {
        return *m_childGeometries.at(i);
    }

    void Geometry::addGeometry(Geometry* geom)
    {
        m_childGeometries.push_back(std::unique_ptr<Geometry>(geom));
    }

    Geometry::GeometryType Geometry::getType() const
    {
        return m_type;
    }

    Geometry::~Geometry()
    {
    }

    unsigned int Geometry::lod() const
    {
        return m_lod;
    }

    void Geometry::setLod(unsigned int lod)
    {
        m_lod = lod;
    }


    void Geometry::addPolygon( Polygon* p )
    {
        m_polygons.push_back( std::unique_ptr<Polygon>(p) );
    }

    void Geometry::finish(bool tesselate, Tesselator& tesselator)
    {
        // only need to finish geometry once
        if (m_finished) {
            throw std::runtime_error("Called Geometry::finish on already finished Geometry.");
        }

        m_finished = true;

        for (std::unique_ptr<Geometry>&  child : m_childGeometries) {
            child->addTargetDefinitionsOf(*this);
            child->finish(tesselate, tesselator);
        }

        for (std::unique_ptr<Polygon>& polygon : m_polygons) {
            polygon->addTargetDefinitionsOf(*this);
            polygon->finish(tesselate, tesselator);
        }

    }

    std::ostream& operator<<( std::ostream& os, const citygml::Geometry& s )
    {
        unsigned int count = 0;
        for ( unsigned int i = 0; i < s.getPolygonsCount(); i++ )
        {
            os << s.getPolygon(i);
            count += s.getPolygon(i).getVertices().size();
        }

        os << "  @ " << s.getPolygonsCount() << " polys [" << count << " vertices]" << std::endl;

        return os;
    }

}
