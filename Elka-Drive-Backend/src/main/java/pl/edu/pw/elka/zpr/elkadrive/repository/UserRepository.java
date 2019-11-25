package pl.edu.pw.elka.zpr.elkadrive.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;
import pl.edu.pw.elka.zpr.elkadrive.domain.UserIdent;

import java.util.Optional;

@Repository
public interface UserRepository extends CrudRepository<UserIdent, Long> {
    Optional<UserIdent> findByUsername(String username);
}
