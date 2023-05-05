package gms.wargrey.model;

public abstract class GMSEntity {
	@Override public abstract int hashCode();
	@Override public abstract String toString();
	
	@Override public final boolean equals(Object entity) {
		if (this == entity) return true;
		if (entity == null) return false;
		if (this.getClass() != entity.getClass()) return false;
		
		return (this.hashCode() == entity.hashCode());
	}
}
